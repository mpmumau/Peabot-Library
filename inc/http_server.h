#ifndef HTTP_SERVER_H_DEF
#define HTTP_SERVER_H_DEF

/*
 File:          http_server.h
 Description:   Declarations for the HTTP server component of Peabot.
 Created:       May 22, 2017
 Author:        Matt Mumau
 */

#define DEFAULT_HTTP_MAX_CONNS 32
#define DEFAULT_HTTP_BUFFER_SIZE 1024
#define DEFAULT_HTTP_RESPONSE_SIZE 32768

#define DEFAULT_HTTP_MAX_BUFFER 262144 // 256kb
#define DEFAULT_HTTP_LINE_LEN 256
#define DEFAULT_HTTP_MAX_HEADERS 31

/* System includes */
#include <stdbool.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>

/* Application includes */
#include "config_defaults.h"

typedef struct HTTPServer {
    bool        running;
    pthread_t   thread;
    int         socket;
    struct      sockaddr_in srv_addr;
    struct      sockaddr_in cli_addr;
    char        buffer[DEFAULT_HTTP_MAX_BUFFER];
} HTTPServer;

/* Initialize the HTTP server component of Peabot. */
void http_init();

/* Halt the HTTP server thread and destroy the component's resources. */
void http_halt();

#endif