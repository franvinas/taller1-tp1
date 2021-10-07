#ifndef COMMON_GAME_STATE_H
#define COMMON_GAME_STATE_H

#include <stdbool.h>

/* 
 *  Esta estructura contiene unicamente información pública del juego
 */
typedef struct {
    char *partial_word;
    unsigned char tries_left;
    unsigned short int len;
    bool game_over;
} game_state_t;

/*
 *  Constructor
 */
int game_state_create(game_state_t *self, 
                      unsigned char tries, 
                      unsigned short int word_len);

/*
 *  Destructor
 */
int game_state_destroy(game_state_t *self);

int game_state_decrease_tries(game_state_t *self);

int game_state_guessed_letter(game_state_t *self, 
                              const char letter, 
                              const  int pos);

int game_state_complete_word(game_state_t *self, const char* word);

int game_state_game_over(const game_state_t *self);

int game_state_get_len(const game_state_t *self);

int game_state_get_tries_left(const game_state_t *self);

int game_state_get_partial_word(const game_state_t *self, char *partial_word);

#endif
