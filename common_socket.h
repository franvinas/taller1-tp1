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

void socket_create(socket_t *self);

void socket_destroy(socket_t *self);

void socket_bind_and_listen(socket_t *self,
                            const char *host,
                            const char *service,
                            int queue_length,
                            bool passive);

void socket_accept(socket_t *listener, socket_t *peer);

void socket_close(socket_t *peer);

int socket_connect(socket_t *self, 
                   const char *host,
                   const char *service, 
                   bool passive);

ssize_t socket_send(socket_t *self, const char *buffer, size_t length);

ssize_t socket_receive(socket_t *self, char *buffer, size_t length);

#endif
