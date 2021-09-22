#include "common_socket.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


/***********************
    Metodos privados
************************/
static void socket_getaddrinfo(socket_t *self, \
                        const char *host, \
                        const char *service, \
                        struct addrinfo **ptr) {
    int s = 0;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
    hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
    hints.ai_flags = AI_PASSIVE;     /* AI_PASSIVE for server           */

    s = getaddrinfo(host, service, &hints, ptr);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(-1);
    }
}

static void socket_reuse_address(socket_t *self, struct addrinfo *ptr) {
    // Activamos la opcion de Reusar la Direccion en caso de que esta
    // no este disponible por un TIME_WAIT
    int s;
    int val = 1;
    s = setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        freeaddrinfo(ptr);
        return;
    }
}


static void socket_bind(socket_t *self, struct addrinfo *ptr) {
    int s = bind(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        freeaddrinfo(ptr);
        return;
    }
}

static void socket_listen(socket_t *self) {
    int s = listen(self->fd, 20);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        return;
    }
}


/***********************
    Metodos publicos
************************/

void socket_create(socket_t *self) {
    self->fd = -1;
    self->address = NULL;
}

void socket_destroy(socket_t *self) {
    freeaddrinfo(self->address);
    close(self->fd);
}

void socket_bind_and_listen(socket_t *self, \
                            const char *host, \
                            const char *service) {
    struct addrinfo *ptr;
    socket_getaddrinfo(self, host, service, &ptr);
    
    self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (self->fd == -1) {
        printf("Error: %s\n", strerror(errno));
        freeaddrinfo(ptr);
        return;
    }

    socket_reuse_address(self, ptr);

    socket_bind(self, ptr);

    freeaddrinfo(ptr);

    socket_listen(self); 
}

void socket_accept(socket_t *listener, socket_t *peer) {
    int peerskt = accept(listener->fd, NULL, NULL);   // aceptamos un cliente
    if (peerskt == -1) {
        printf("Error: %s\n", strerror(errno));
    }
    peer->fd = peerskt;
}

int socket_connect(socket_t *self, const char *host, const char *service) {
    struct addrinfo *ptr;
    
    socket_getaddrinfo(self, host, service, &ptr);

    self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (self->fd == -1) {
        printf("Error: %s\n", strerror(errno));
        freeaddrinfo(ptr);
        return -1;
    }

    int c = connect(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (c == -1) {
        printf("Error: %s\n", strerror(errno));
        freeaddrinfo(ptr);
        return -1;
    }
    self->address = ptr;
    return 0;
}

ssize_t socket_send(socket_t *self, const char *buffer, size_t length) {
    ssize_t w = write(self->fd, buffer, length);
    if (w == -1) {
        printf("Error write: %s\n", strerror(errno));
        return w;
    }

    return w;
}

ssize_t socket_receive(socket_t *self, char *buffer, size_t length) {
    ssize_t w = read(self->fd, buffer, length);
    if (w == -1) {
        printf("Error read: %s", strerror(errno));
        return w;
    }

    return w;
}
