#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "common_socket.h"

bool message_decode_and_print(char *msg) {
    bool game_over = (msg[0] & 0x80);
    unsigned char tries_left = (msg[0] & 0x3F);
    unsigned short int msg_len_net = msg[2] | (msg[1] << 8);
    unsigned short int msg_len = ntohs(msg_len_net);
    char *word = msg + 3;
    word[msg_len] = '\0';

    if (!game_over) {
        printf("Palabra seceta: %s\n", word);
        printf("Te quedan %hhu intentos\n", tries_left);
    } else if (tries_left > 0) {
        printf("Ganaste!!\n");
    } else {
        printf("Perdiste! La palabra era: %s\n", word);
    }
    return game_over;    
}

int main(int argc, const char *argv[]) {
    socket_t client_socket;
    char c;
    char msg[10];
    bool game_over = false;
    socket_create(&client_socket);
    socket_connect(&client_socket, "127.0.0.1", "7778");
    

    while (!game_over) {
        if (c != '\n')
            printf("Enter character: ");
        c = getchar();
        if (c != '\n') {
            socket_send(&client_socket, &c, 1);
            socket_receive(&client_socket, msg, 10);
            game_over = message_decode_and_print(msg);
        }
    }    

    socket_destroy(&client_socket);

    return 0;
}
