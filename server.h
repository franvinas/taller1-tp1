#include <stdio.h>
#include "common_socket.h"
#include "server_hangman.h"

#ifndef SERVER_H
#define SERVER_H

typedef struct {
    FILE *words_repo;
    socket_t sk;
    unsigned int victories;
    unsigned int defeats;
    int tries;
} server_t;


int server_create(server_t *self, 
                  const char *port, 
                  int tries, 
                  const char *words_repo_name);

int server_run(server_t *self);

void server_print_summary(server_t *self);

int server_destroy(server_t *self);

#endif
