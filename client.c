#include "client.h"
#include "common_socket.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define MIN_LEN_MSG 3

/***********************
    Metodos privados
************************/

unsigned short int get_word_len(char *msg) {
    unsigned short int word_len_net;
    memcpy(&word_len_net, &msg[1], 2);
    return ntohs(word_len_net);
}

bool message_decode_and_print(char *msg, unsigned short int word_len) {
    bool game_over = (msg[0] & 0x80) != 0;
    unsigned char tries_left = (msg[0] & 0x3F);
    char *word = msg + 3;
    word[word_len + 3] = '\0';

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

/***********************
    Metodos publicos
************************/

int client_create(client_t *self, const char *host, const char *port) {
    bool passive = false;
    int s;
    socket_create(&self->sk);
    s = socket_connect(&self->sk, host, port, passive);
    if (s == -1) {
        return 1;
    }
    return 0;
}


int client_run(client_t *self) {
    unsigned short int word_len;
    char c = 0;
    char msg[100] = "";
    bool game_over = false;

    socket_receive(&self->sk, msg, MIN_LEN_MSG);
    word_len = get_word_len(msg);
    socket_receive(&self->sk, msg + MIN_LEN_MSG, word_len);
    game_over = message_decode_and_print(msg, word_len);

    while (!game_over) {
        if (c != '\n')
            printf("Ingrese letra: ");
        c = getchar();
        if (c != '\n') {
            printf("\n");
            socket_send(&self->sk, &c, 1);
            socket_receive(&self->sk, msg, word_len + MIN_LEN_MSG);
            game_over = message_decode_and_print(msg, word_len);
        }
    }

    return 0;
}


int client_destroy(client_t *self) {
    socket_destroy(&self->sk);
    
    return 0;
}
