#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/***********************
    Metodos privados
************************/

void print_player_msg(const bool game_over, 
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
}

/***********************
    Metodos publicos
************************/

int client_create(client_t *self, const char *host, const char *port) {
    if (protocol_create(&self->protocol) != 0) return -1;
    if (protocol_connect(&self->protocol, host, port) != 0) {
        protocol_destroy(&self->protocol);
        return -1;
    }
    return 0;
}

int client_destroy(client_t *self) {
    if (protocol_destroy(&self->protocol) != 0) return -1;
    return 0;
}

int client_run(client_t *self) {
    char c = 0;
    char *word = NULL;
    bool game_over = false;
    unsigned char tries_left;

    int r = protocol_client_recv(&self->protocol, 
                                 &word, 
                                 &tries_left, 
                                 &game_over);
    if (r != 0) {
        free(word);
        return -1;
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
                return -1;
            }
            
            r = protocol_client_recv(&self->protocol, 
                                     &word, 
                                     &tries_left, 
                                     &game_over);
            if (r != 0) {
                free(word);
                return -1;
            }
            print_player_msg(game_over, tries_left, word);
        }
    }
    
    free(word);

    return 0;
}
