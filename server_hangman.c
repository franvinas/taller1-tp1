#include "server_hangman.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MIN_LEN_MSG 3

/***********************
    Metodos privados
************************/

static void hangman_initialize_partial_word(hangman_t *self) {
    int i = 0;
    for (; i < self->len; i++)
        self->partial_word[i] = '_';
}

static bool hangman_contains_letter(hangman_t *self, char c) {
    bool guess = false;
    for (int i = 0; i < self->len; i++) {
        if (self->word[i] == c) {
            guess = true;
            self->partial_word[i] = c;
        }
    }
    return guess;
}

/***********************
    Metodos publicos
************************/

int hangman_create(hangman_t *self, char *word, int tries) {
    self->tries_left = tries;
    self->game_over = false;
    self->len = strlen(word);
    self->word = word;
    self->msg = malloc(self->len + MIN_LEN_MSG);
    self->partial_word = self->msg + MIN_LEN_MSG;
    memcpy(self->word, word, self->len);
    hangman_initialize_partial_word(self);

    return 0;
}

int hangman_destroy(hangman_t *self) {
    free(self->msg);
    return 0;
}

int hangman_try_letter(hangman_t *self, char letter) {
    bool guess = hangman_contains_letter(self, letter);
    if (!guess) {
        self->tries_left -=1;
        if (self->tries_left == 0) {
            self->game_over = true;
            memcpy(self->partial_word, self->word, self->len);
            return 1;
        }
    } else if (!memcmp(self->word, self->partial_word, self->len)) {
        self->game_over = true;
        return 1;
    }

    return 0;
}

int hangman_get_msg(hangman_t *self, char **msg) {
    int msg_len = 3 + self->len;
    unsigned short int word_len_be = htons(self->len);
    
    self->msg[0] = (char) (self->game_over ? 0x80 : 0);
    self->msg[0] = self->msg[0] | self->tries_left;
    
    memcpy(&self->msg[1], &word_len_be, 2);

    *msg = self->msg;

    return msg_len;
}

