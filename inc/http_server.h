#ifndef HTTP_SERVER_H_DEF
#define HTTP_SERVER_H_DEF

/*
 File:          http_server.h
 Description:   Declarations for the HTTP server component of Peabot.
 Created:       May 22, 2017
 Author:        Matt Mumau
 */

#define HTTP_SERVER_MAX_CONNS 1024
#define HTTP_SERVER_BUFFER_LEN (256*1024) // 256kb

/* System includes */
#include <stdbool.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>

/* Application includes */
#include "config_defaults.h"
#include "http_request.h"

typedef struct HTTPServer {
    int         socket;
    struct      sockaddr_in srv_addr;
    struct      sockaddr_in cli_addr;
    size_t      client_len;
    char        buffer[HTTP_SERVER_BUFFER_LEN];
} HTTPServer;

typedef struct HTTPRequestData {
    HTTPRequest *http_request;
    int socket_fd;
} HTTPRequestData;

/* Initialize the HTTP server component of Peabot. */
void http_init();

/* Halt the HTTP server thread and destroy the component's resources. */
void http_halt();

#endif