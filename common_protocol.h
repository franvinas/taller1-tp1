#ifndef COMMON_PROTOCOL_H
#define COMMON_PROTOCOL_H

#include "common_socket.h"
#include "common_game_state.h"

typedef struct {
    socket_t sk;
    socket_t peer_sk;
} protocol_t;

/*
 *  Constructor
 */
int protocol_create(protocol_t *self);

/*
 *  Destructor
 */
int protocol_destroy(protocol_t *self);

/*
 *  Se debe llamar desde el servidor para esperar un cliente.
 */
int protocol_listen(protocol_t *self, const char *port);

/*
 *  Acepta un nuevo cliente.
 */
int protocol_new_client(protocol_t *self);

/*
 *  Termina la conexión con el cliente.
 */
int protocol_free_client(protocol_t *self);

/*
 *  Se debe llamar desde el cliente para conectarse a un servidor dado 
 *  el 'host' y 'port' indicados.
 */
int protocol_connect(protocol_t *self, const char *host, const char *port);

/*
 *  Se debe llamar desde el servidor para enviar un mensaje al cliente con el
 *  protocolo (manejo de bits) adecuado.
 */
int protocol_server_send(protocol_t *self, const game_state_t *game_state);

/*
 *  Se debe llamar desde el servidor. Recibe un caracter 'c'.
 */
int protocol_server_recv(protocol_t *self, char *c);

/*
 *  Se debe llamar desde el cliente. Envía un caracter 'c'.
 */
int protocol_client_send(protocol_t *self, char c);

/*
 *  Se debe llamar desde el cliente para recibir un mensaje al servidor con el
 *  protocolo (manejo de bits) adecuado. Se almacenan los datos extraídos del 
 *  mensaje recibido en 'partial_word', 'tries_left' y 'game_over'.
 *  Si 'partial_word' es un puntero a NULL se aloca la memoria necesaria. Es 
 *  responsabilidad del usuario de la función liberar esta memoria.
 */
int protocol_client_recv(protocol_t *self, 
                         char **partial_word,
                         unsigned char *tries_left,
                         bool *game_over);

#endif
