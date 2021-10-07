#include "common_game_state.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>

/***********************
    Metodos publicos
************************/

int game_state_create(game_state_t *self,
                      unsigned char tries, 
                      unsigned short int word_len) {
    self->tries_left = tries;
    self->game_over = false;
    self->len = word_len;
    self->partial_word = malloc(word_len);
    if (self->partial_word == NULL) {
        return 1;
    }
    
    for (int i = 0; i < self->len; i++)
        self->partial_word[i] = '_';

    return 0;
}

int game_state_destroy(game_state_t *self) {
    free(self->partial_word);
    return 0;
}

int game_state_guessed_letter(game_state_t *self, 
                              const char letter, 
                              const int pos) {
    self->partial_word[pos] = letter;
    if (memchr(self->partial_word, '_', self->len) == NULL)
        self->game_over = true;

    return 0;
}

int game_state_decrease_tries(game_state_t *self) {
    self->tries_left -= 1;
    if (self->tries_left == 0)
        self->game_over = true;
    return 0;
}

int game_state_complete_word(game_state_t *self, const char* word) {
    memcpy(self->partial_word, word, self->len);
    return 0;
}

int game_state_game_over(const game_state_t *self) {
    return self->game_over;
}

int game_state_get_len(const game_state_t *self) {
    return self->len;
}

int game_state_get_tries_left(const game_state_t *self) {
    return self->tries_left;
}

int game_state_get_partial_word(const game_state_t *self, char *partial_word) {
    memcpy(partial_word, self->partial_word, self->len);
    return 0;
}
