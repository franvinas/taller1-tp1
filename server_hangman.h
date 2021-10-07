#ifndef SERVER_HANGMAN_H
#define SERVER_HANGMAN_H

#include <stdbool.h>
#include "common_game_state.h"

typedef struct {
    char *word;
    unsigned short int len;
    game_state_t game_state;
} hangman_t;

/*
 *  Constructor
 *  Recibe la palabra a adivinar. Es responsabilidad del usuario alocar la 
 *  palabra en el heap y liberarla posteriormente.
 *  Además recibe la cantidad de intentos que tiene el jugador.
 */
int hangman_create(hangman_t *self, char *word, unsigned char tries);

/*
 *  Destructor
 */
int hangman_destroy(hangman_t *self);

/*
 *  Recibe un caracter. Si la letra pertenece a la palabra secreta entonces 
 *  se actualiza la palabra parcial.
 *  Si la letra no pertenece a la palabra el jugador pierde un intento.
 *  En ambos casos la función retorna un 1 si el juego terminó o 0 si el juego
 *  aun no terminó.
 */
int hangman_try_letter(hangman_t *self, char letter);

/*
 *  Esta función recibe el argumento "msg" donde se guarda una dirección del 
 *  heap donde se aloja el mensaje codificado segun el protocolo de 
 *  comunicacion.
 *  La memoria utilizada para el mensaje se libera llamando al destructor.
 */
int hangman_get_msg(hangman_t *self, char **msg);

game_state_t *hangman_get_game_state(hangman_t *self);

#endif
