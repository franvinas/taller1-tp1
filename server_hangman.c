#include "server_hangman.h"
#include <string.h>

#define MIN_LEN_MSG 3

/***********************
    Metodos privados
************************/

/*
 *  Retorna 'true' si la palabra secreta contiene la letra 'c'.
 *  Actualiza la palabra parcial en caso de 'true'.
 */
static bool hangman_contains_letter(hangman_t *self, char c) {
    bool guess = false;
    for (int i = 0; i < self->len; i++) {
        if (self->word[i] == c) {
            guess = true;
            game_state_guessed_letter(&self->game_state, c, i);
        }
    }
    return guess;
}

/***********************
    Metodos publicos
************************/

int hangman_create(hangman_t *self, char *word, unsigned char tries) {
    self->len = strlen(word);
    self->word = word;
    return game_state_create(&self->game_state, tries, self->len);
}

int hangman_destroy(hangman_t *self) {
    return game_state_destroy(&self->game_state);
}

int hangman_try_letter(hangman_t *self, char letter) {
    if (!hangman_contains_letter(self, letter))
        game_state_decrease_tries(&self->game_state);
    
    if (game_state_game_over(&self->game_state)) {
        game_state_complete_word(&self->game_state, self->word);
        return -1;
    }
    return 0;
}

game_state_t *hangman_get_game_state(hangman_t *self) {
    return &self->game_state;
}
