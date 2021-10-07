#include "client.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/***********************
    Metodos privados
************************/

// static int client_get_msg(client_t *self, 
//                           unsigned char *game_info, 
//                           uint16_t *word_len, 
//                           char *word) {
//     int s;
//     s = socket_recv(&self->sk, (char *) game_info, 1);
//     if (s != 0) return 1;

//     s = socket_recv(&self->sk, (char *) word_len, 2);
//     if (s != 0) return 1;

//     *word_len = ntohs(*word_len);

//     s = socket_recv(&self->sk, word, *word_len);
//     if (s != 0) return 1;

//     word[*word_len] = '\0';

//     return 0;
// }

// static bool message_decode_and_print(const unsigned char game_info, 
//                                      const uint16_t word_len, 
//                                      const char *word) {
//     bool game_over = (game_info & 0x80) != 0;
//     unsigned char tries_left = (game_info & 0x7F);

//     if (!game_over) {
//         printf("Palabra secreta: %s\n", word);
//         printf("Te quedan %hhu intentos\n", tries_left);
//     } else if (tries_left > 0) {
//         printf("Ganaste!!\n");
//     } else {
//         printf("Perdiste! La palabra secreta era: '%s'\n", word);
//     }
//     return game_over;    
// }

int print_player_msg(const bool game_over, 
                      const unsigned char tries_left, 
                      const char *word) {
    if (!game_over) {
        printf("Palabra secreta: %s\n", word);
        printf("Te quedan %hhu intentos\n", tries_left);
    } else if (tries_left > 0) {
        printf("Ganaste!!\n");
    } else {
        printf("Perdiste! La palabra secreta era: '%s'\n", word);
    }
    return 0;
}

/***********************
    Metodos publicos
************************/

int client_create(client_t *self, const char *host, const char *port) {
    if (protocol_create(&self->protocol) != 0) return 1;
    if (protocol_connect(&self->protocol, host, port)) return 1;
    return 0;
}

int client_destroy(client_t *self) {
    if (protocol_destroy(&self->protocol) != 0) return 1;
    return 0;
}

int client_run(client_t *self) {
    char c = 0;
    char *word;
    bool game_over = false;
    uint16_t word_len = 0;
    unsigned char tries_left;

    word = malloc(word_len + 1);
    if (word == NULL) {
        return 1;
    }

    int r = protocol_client_recv(&self->protocol, 
                                 word, 
                                 &tries_left, 
                                 &game_over);
    if (r != 0) {
        free(word);
        return 1;
    }
    
    print_player_msg(game_over, tries_left, word);

    while (!game_over) {
        if (c != '\n')
            printf("Ingrese letra: ");
        c = getchar();
        if (c != '\n') {
            printf("\n");
            if (protocol_client_send(&self->protocol, c) != 0) {
                free(word);
                return 1;
            }
            
            r = protocol_client_recv(&self->protocol, 
                                     word, 
                                     &tries_left, 
                                     &game_over);
            if (r != 0) {
                free(word);
                return 1;
            }
            print_player_msg(game_over, tries_left, word);
        }
    }
    
    free(word);

    return 0;
}
