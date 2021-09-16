#include <stdbool.h>

#ifndef SERVER_AHORCADO_H
#define SERVER_HANGMAN_H

#define WORD_MAX_SIZE 15

typedef struct {
    char word[WORD_MAX_SIZE];
    char partial_word[WORD_MAX_SIZE];
    int tries_left;
    int len;

} hangman_t;


int hangman_create(hangman_t *self, char *word);

int hangman_destroy(hangman_t *self);

int hangman_try_letter(hangman_t *self, char letter);

bool hangman_contains_letter(hangman_t *self, char c);

void hangman_initialize_partial_word(hangman_t *self);

void hangman_show_info(hangman_t *self);

#endif