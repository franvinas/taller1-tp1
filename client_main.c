#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "common_socket.h"

#define MAX_LENGTH_MSG 100

bool message_decode_and_print(char *msg) {
    bool game_over = (msg[0] & 0x80);
    unsigned char tries_left = (msg[0] & 0x3F);
    unsigned short int msg_len_net = msg[2] | (msg[1] << 8);
    unsigned short int msg_len = ntohs(msg_len_net);
    char *word = msg + 3;
    word[msg_len] = '\0';

    if (!game_over) {
        printf("Palabra secreta: %s\n", word);
        printf("Te quedan %hhu intentos\n", tries_left);
    } else if (tries_left > 0) {
        printf("Ganaste!!\n");
    } else {
        printf("Perdiste! La palabra secreta era: '%s'\n", word);
    }
    return game_over;    
}

int main(int argc, const char *argv[]) {
    socket_t client_socket;
    int s;
    char c = 0;
    char msg[100] = "";
    bool game_over = false;
    bool passive = false;
    
    if (argc != 3) {
        printf("Error en la cantidad de argumentos\n");
        printf("El cliente se ejecuta de la siguiente manera:\n");
        printf("./client <host> <port>\n");
        return 1;
    }

    socket_create(&client_socket);
    s = socket_connect(&client_socket, argv[1], argv[2], passive);
    if (s == -1) {
        return 1;
    }

    socket_receive(&client_socket, msg, MAX_LENGTH_MSG);
    game_over = message_decode_and_print(msg);

    while (!game_over) {
        if (c != '\n')
            printf("Ingrese letra: ");
        c = getchar();
        if (c != '\n') {
            printf("\n");
            socket_send(&client_socket, &c, 1);
            socket_receive(&client_socket, msg, MAX_LENGTH_MSG);
            game_over = message_decode_and_print(msg);
        }
    }

    socket_destroy(&client_socket);

    return 0;
}
