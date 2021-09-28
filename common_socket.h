#ifndef SOCKET_H
#define SOCKET_H

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int fd;
} socket_t;

/*
 *  Constructor
 */
int socket_create(socket_t *self);

/*
 *  Destructor
 */
int socket_destroy(socket_t *self);

/*
 *  Asigna IP ("host") y numero de puerto ("service") al socket.
 *  "queue_length" indica la cantidad de cliente que se quiere mantener en
 *  espera. Se atiende a un cliente a la vez.
 */
int socket_bind_and_listen(socket_t *self,
                            const char *host,
                            const char *service,
                            int queue_length);

/*
 *  El socket "self" acepta al socket "peer". Una vez que la conexión finaliza
 *  el socket peer debe ser destruido.
 */
int socket_accept(socket_t *self, socket_t *peer);

/*
 *  El socket se conecta la IP dada por "host" en el puerto dado por "service".
 */
int socket_connect(socket_t *self, 
                   const char *host,
                   const char *service);

/*
 *  Se envian una cantidad de bytes (dada por len) del "buffer".
 *  Se asegura que todos los bytes se envian.
 *  Si ocurre un error la funcion retorna un valor distinto de 0.
 */
int socket_send(socket_t *self, const char *buffer, size_t len);

/*
 *  Se recibe una cantidad de bytes (dada por len) y se guarda en el "buffer".
 *  Se asegura que todos los bytes se reciben.
 *  Si ocurre un error la funcion retorna un valor distinto de 0.
 */
int socket_recv(socket_t *self, char *buffer, size_t len);

#endif
