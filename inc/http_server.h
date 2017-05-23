#ifndef HTTP_SERVER_H_DEF
#define HTTP_SERVER_H_DEF

/*
 File:          http_server.h
 Description:   Declarations for the HTTP server component of Peabot.
 Created:       May 22, 2017
 Author:        Matt Mumau
 */

#include <stdbool.h>
#include <sys/socket.h>

typedef struct HTTPServer {
    bool running;
    pthread_t thread;
    int socket;
    struct sockaddr_in srv_addr;
    struct sockaddr_in cli_addr;
} HTTPServer;

/* Initialize the HTTP server component of Peabot. */
void http_init();

/* Halt the HTTP server thread and destroy the component's resources. */
void http_halt();

#endif