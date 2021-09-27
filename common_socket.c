#include "common_socket.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


/***********************
    Metodos privados
************************/

static void socket_getaddrinfo(socket_t *self,
                        const char *host,
                        const char *service,
                        struct addrinfo **ptr,
                        bool passive) {
    int s = 0;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;                      /* IPv4 */
    hints.ai_socktype = SOCK_STREAM;                /* TCP  */
    hints.ai_flags = passive ? AI_PASSIVE : 0;      /* AI_PASSIVE for server */
                                                    /* and 0 for client */
    s = getaddrinfo(host, service, &hints, ptr);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(-1);
    }
}

// Activamos la opcion de Reusar la Direccion en caso de que esta
// no este disponible por un TIME_WAIT    
static void socket_reuse_address(socket_t *self, struct addrinfo *ptr) {
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

static void socket_listen(socket_t *self, int queue_length) {
    int s = listen(self->fd, queue_length);
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
}

void socket_destroy(socket_t *self) {
    close(self->fd);
}

void socket_bind_and_listen(socket_t *self,
                            const char *host,
                            const char *service,
                            int queue_length,
                            bool passive) {
    struct addrinfo *ptr;
    socket_getaddrinfo(self, host, service, &ptr, passive);
    
    self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (self->fd == -1) {
        printf("Error: %s\n", strerror(errno));
        freeaddrinfo(ptr);
        return;
    }

    socket_reuse_address(self, ptr);
    socket_bind(self, ptr);
    freeaddrinfo(ptr);
    socket_listen(self, queue_length); 
}

void socket_accept(socket_t *listener, socket_t *peer) {
    int peerskt = accept(listener->fd, NULL, NULL);
    if (peerskt == -1) {
        printf("Error: %s\n", strerror(errno));
    }
    peer->fd = peerskt;
}

void socket_close(socket_t *peer) {
    close(peer->fd);
}

int socket_connect(socket_t *self, 
                    const char *host, 
                    const char *service, 
                    bool passive) {
    struct addrinfo *ptr;
    
    socket_getaddrinfo(self, host, service, &ptr, passive);
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
    freeaddrinfo(ptr);
    return 0;
}

ssize_t socket_send(socket_t *self, const char *buffer, size_t len) {
    ssize_t sent_b = 0;

    while (len > sent_b) {
        ssize_t b = send(self->fd, buffer + sent_b, len - sent_b, MSG_NOSIGNAL);
        if (b == -1) {
            printf("Error write: %s\n", strerror(errno));
            return b;
        } else if (b == 0) { // Socket cerrado
            return 0;
        } else {
            sent_b += b;
        }
    }
    return 0;
}

ssize_t socket_receive(socket_t *self, char *buffer, size_t len) {
    ssize_t recv_b = 0;

    while (len > recv_b) {
        ssize_t b = recv(self->fd, buffer + recv_b, len - recv_b, 0);
        if (b == -1) {
            printf("Error read: %s\n", strerror(errno));
            return b;
        } else if (b == 0) { // Socket cerrado
            return 0;
        } else {
            recv_b += b;
        }
    }
    return 0;
}
