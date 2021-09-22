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
    
    return msg_len;
}

int hangman_create(hangman_t *self, char *word, int tries) {
    self->tries_left = tries;
    self->game_over = false;
    self->len = strlen(word);
    self->word = malloc((self->len));
    self->partial_word = malloc((self->len));
    snprintf(self->word, self->len + 1, "%s", word);
    hangman_initialize_partial_word(self);

    return 0;
}

int hangman_destroy(hangman_t *self) {
    free(self->word);
    return 0;
}

int hangman_try_letter(hangman_t *self, char letter) {
    bool guess = hangman_contains_letter(self, letter);
    if (!guess) {
        self->tries_left -=1;
        if (self->tries_left == 0) {
            self->game_over = true;
            snprintf(self->partial_word, self->len + 1, "%s", self->word);
            return -1;
        }
    } else if (!strcmp(self->word, self->partial_word)) {
        self->game_over = true;
        return -1;
    }

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
