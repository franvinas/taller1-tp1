#include "common_socket.h"
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/***********************
    Metodos privados
************************/

static int socket_getaddrinfo(socket_t *self,
                        const char *host,
                        const char *service,
                        struct addrinfo **ptr,
                        bool passive) {
    int s = 0;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;                  /* IPv4 */
    hints.ai_socktype = SOCK_STREAM;            /* TCP  */
    hints.ai_flags = passive ? AI_PASSIVE : 0;  /* AI_PASSIVE para el server */
                                                /* y 0 para el cliente */
    s = getaddrinfo(host, service, &hints, ptr);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return -1;
    }
    return 0;
}

/*  Se activa la opción de reusar la direccion en caso de que esta
 *  no este disponible por un TIME_WAIT 
 */
static int socket_reuse_address(socket_t *self) {
    int val = 1;
    int s = setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (s == -1) return -1;
    return 0;
}

static int socket_bind(socket_t *self, struct addrinfo *ptr) {
    int s = bind(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

static int socket_listen(socket_t *self, int queue_length) {
    int s = listen(self->fd, queue_length);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        return -1;
    }
    return 0;
}

/***********************
    Metodos publicos
************************/

int socket_create(socket_t *self) {
    self->fd = -1;
    return 0;
}

int socket_destroy(socket_t *self) {
    int i = 0;
    if (self->fd != -1)
        i = close(self->fd);
    if (i != 0) return -1; // falló close
    return 0;
}

int socket_bind_and_listen(socket_t *self,
                            const char *host,
                            const char *service,
                            int queue_length) {
    struct addrinfo *ptr, *rp;
    bool passive = true; // necesario para luego aceptar conexiones
    
    if (socket_getaddrinfo(self, host, service, &ptr, passive) != 0)
        return -1;

    for (rp = ptr; rp != NULL; rp = rp->ai_next) {
        self->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (self->fd == -1)
            continue;

        if (socket_bind(self, rp) == 0)
            break; // Bind exitoso

        close(self->fd);
    }
    freeaddrinfo(ptr);
    
    if (rp == NULL) { // Falló bind para todas las addrs
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        return -1;
    }

    if (socket_reuse_address(self) != 0) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        return -1;
    }
    
    if (socket_listen(self, queue_length) != 0) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        return -1;
    }
    return 0;
}

int socket_accept(socket_t *self, socket_t *peer) {
    peer->fd = accept(self->fd, NULL, NULL);
    if (peer->fd == -1) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int socket_connect(socket_t *self, 
                    const char *host, 
                    const char *service) {
    struct addrinfo *ptr, *rp;
    bool passive = false; // necesario para luego usar connect
    
    if (socket_getaddrinfo(self, host, service, &ptr, passive) != 0)
        return -1;

    for (rp = ptr; rp != NULL; rp = rp->ai_next) {
        self->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (self->fd == -1)
            continue;

        if (connect(self->fd, ptr->ai_addr, ptr->ai_addrlen) == 0)
            break; // Connect exitoso

        close(self->fd);
    }
    freeaddrinfo(ptr);
    
    if (rp == NULL) { // Falló connect para todas las addrs
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        return -1;
    }

    return 0;
}

int socket_send(socket_t *self, const char *buffer, size_t len) {
    ssize_t sent_b = 0;
    while (len > sent_b) {
        ssize_t b = send(self->fd, buffer + sent_b, len - sent_b, MSG_NOSIGNAL);
        if (b == -1) {
            printf("Error send: %s\n", strerror(errno));
            return -1;
        } else if (b == 0) { // Socket cerrado
            return 0;
        } else {
            sent_b += b;
        }
    }
    return 0;
}

int socket_recv(socket_t *self, char *buffer, size_t len) {
    ssize_t recv_b = 0;

    while (len > recv_b) {
        ssize_t b = recv(self->fd, buffer + recv_b, len - recv_b, 0);
        if (b == -1) {
            printf("Error recv: %s\n", strerror(errno));
            return -1;
        } else if (b == 0) { // Socket cerrado
            return 0;
        } else {
            recv_b += b;
        }
    }
    return 0;
}
