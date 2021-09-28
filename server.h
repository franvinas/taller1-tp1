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

/*
 *  Constructor
 */
int server_create(server_t *self);

/*
 *  Destructor
 */
int server_destroy(server_t *self);

/*
 *  Asigna la IP local y el puerto dado por argumento al socket
 */
int server_bind_and_listen(server_t *self, const char *port);

/*
 *  Abre el archivo de nombre "words_repo_name" y por cada linea se queda a la
 *  espera de que un cliente se conecte para jugar. Una vez que se conecta un
 *  cliente se crea una partida de ahorcado con la palabra de la primera linea
 *  del archivo y la cantidad de intentos dado por "tries".
 *  La ejecucion de la funcion finaliza una vez que no hay mas palabras o
 *  cuando ocurre un error.
 *  Devuelve 0 si se ejecuto correctamente o un entero distinto de 0 en caso
 *  contrario.
 */
int server_run(server_t *self, int tries, const char *words_repo_name);

/*
 *  Imprime por salida estandar un resumen de las partidas jugadas, indicando
 *  cantidad de victorias y cantidad de derrotas.
 */
void server_print_summary(server_t *self);

#endif
