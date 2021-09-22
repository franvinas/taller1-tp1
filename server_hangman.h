#include <stdbool.h>

#ifndef SERVER_AHORCADO_H
#define SERVER_HANGMAN_H

typedef struct {
    char *word;
    char *partial_word;
    unsigned char tries_left;
    unsigned short int len;
    bool game_over;
} hangman_t;


int hangman_create(hangman_t *self, char *word, int tries);

int hangman_destroy(hangman_t *self);

int hangman_try_letter(hangman_t *self, char letter);

int hangman_get_msg(hangman_t *self, char *msg);

#endif
