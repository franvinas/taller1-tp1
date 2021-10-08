#include "server.h"
#include "server_hangman.h"
#include <stdlib.h>

/***********************
    Metodos privados
************************/

/*
 *  Se juega una partida de ahorcado. 
 *  Si ocurre un error retorna -1
 *  Si al finalizar la partida, el jugador adivinó la palabra retorna 1
 *  Si al finalizar la partida, el jugador mp adivinó la palabra retorna 0
 */
static int play_hangman(hangman_t *hangman, protocol_t *protocol) {
    char c;
    game_state_t *game_state = hangman_get_game_state(hangman);
    if (protocol_server_send(protocol, game_state) != 0) return 1;

    while (!game_state_game_over(game_state)) {
        if (protocol_server_recv(protocol, &c) != 0) return 1;
        hangman_try_letter(hangman, c);
        game_state = hangman_get_game_state(hangman);
    if (protocol_server_send(protocol, game_state) != 0) return 1;
    }
    return game_state_get_tries_left(game_state) > 0 ? 1 : 0;
}

/***********************
    Metodos publicos
************************/

int server_create(server_t *self, const char *port) {
    if (protocol_create(&self->protocol) != 0) return 1;
    if (protocol_listen(&self->protocol, port) != 0) {
        protocol_destroy(&self->protocol);
        return 1;
    }
    if (summary_create(&self->summary) != 0) {
        protocol_destroy(&self->protocol);
        return 1;
    }

    self->words_repo = NULL;
    self->tries = 0;
    return 0;
}

int server_destroy(server_t *self) {
    protocol_destroy(&self->protocol);
    summary_destroy(&self->summary);
    if (self->words_repo)
        fclose(self->words_repo);
    return 0;
}

int server_run(server_t *self, int tries, const char *words_repo_name) {
    char *word = NULL;
    ssize_t line_len = 0;
    size_t buffer_size = 0;

    self->tries = tries;
    self->words_repo = fopen(words_repo_name, "r");
    if (self->words_repo == NULL) {
        printf("Error al abrir el archivo\n");
        return 1;
    }
    
    while ((line_len = getline(&word, &buffer_size, self->words_repo)) > 1) {
        hangman_t hangman;
        protocol_new_client(&self->protocol);
        word[line_len - 1] = '\0';
        hangman_create(&hangman, word, self->tries);
        int h = play_hangman(&hangman, &self->protocol);
        if (h == -1) // error
            return 1;
        else if (h == 1)
            summary_new_victory(&self->summary);
        else
            summary_new_defeat(&self->summary);
        hangman_destroy(&hangman);
        protocol_free_client(&self->protocol);
    }

    free(word);
    return 0;
}

void server_print_summary(server_t *self) {
    summary_print(&self->summary);
}