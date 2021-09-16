#include "server_hangman.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int hangman_create(hangman_t *self, char *word) {
    self->tries_left = 5;
    strcpy(self->word, word);
    self->len = strlen(word);
    hangman_initialize_partial_word(self);
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
            printf("Perdiste! La palabra secreta era: '%s'\n", self->word);
            return -1;
        }
    } else if (!strcmp(self->word, self->partial_word)) {
        printf("Ganaste!!\n");
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
    printf("Te quedan %d intentos\n", self->tries_left);
}