#include "server_hangman.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>

int hangman_get_msg(hangman_t *self, char *msg) {
    int msg_len = 3 + self->len;
    short int msg_len_be = htons(msg_len);
    
    msg[0] = (char) (self->game_over ? 0x80 : 0);
    msg[0] = msg[0] | self->tries_left;
    
    msg[1] = (msg_len_be >> 8);
    msg[2] = (msg_len_be & 255);
    for (int i = 0; i < self->len; i++)
        msg[3+i] = self->partial_word[i];
    // printBits(self->msg[0], 1);
    // printBits(self->msg[1], 1);
    // printBits(self->msg[2], 1);
    // msg = self->msg;
    return msg_len;
}


int hangman_create(hangman_t *self, char *word) {
    self->tries_left = 5; // se pasa como argumento
    self->game_over = false;
    strcpy(self->word, word);
    self->len = strlen(word);
    // self->msg = malloc((self->len) + 2);
    hangman_initialize_partial_word(self);
    hangman_show_info(self);
    return 0;
}

int hangman_destroy(hangman_t *self) {
    
    return 0;
}

int hangman_try_letter(hangman_t *self, char letter) {
    bool guess = hangman_contains_letter(self, letter);
    if (!guess) {
        self->tries_left -=1;
        if (self->tries_left == 0) {
            // printf("Perdiste! La palabra secreta era: '%s'\n", self->word);
            self->game_over = true;
            return -1;
        }
    } else if (!strcmp(self->word, self->partial_word)) {
        // printf("Ganaste!!\n");
        self->game_over = true;
        return -1;
    }
    hangman_show_info(self);
    return 0;
}

bool hangman_contains_letter(hangman_t *self, char c) {
    bool guess = false;
    for (int i = 0; i < self->len; i++) {
        if (self->word[i] == c) {
            guess = true;
            self->partial_word[i] = c;
        }
    }
    return guess;
}

void hangman_initialize_partial_word(hangman_t *self) {
    int i = 0;
    for (; i < self->len; i++)
        self->partial_word[i] = '_';
    self->partial_word[i] = '\0';
}

void hangman_show_info(hangman_t *self) {
    printf("Palabra secreta: %s\n", self->partial_word);
    printf("Palabra completa: %s\n", self->word);
    printf("Te quedan %d intentos\n", self->tries_left);
}
