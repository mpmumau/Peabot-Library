#ifndef HTTP_SERVER_DEF
#define HTTP_SERVER_DEF

/*
 File:          http_server.c
 Description:   Implementation of Peabot's HTTP server component.
 Created:       May 22, 2017
 Author:        Matt Mumau
 */

#define _POSIX_C_SOURCE 199309L

/* System includes */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h> 
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>

/* Application includes */
#include "config.h"
#include "config_defaults.h"
#include "main.h"
#include "log.h"
#include "console.h"
#include "utils.h"
#include "http_request.h"
#include "http_response.h"

/* Header */
#include "http_server.h"

/* Forward decs */
static void *http_main(void *arg);
static void http_server_log_connection();

static HTTPServer http;
static socklen_t client_length;

void http_init()
{  
    bool *http_enabled = (bool *) config_get(CONF_HTTP_ENABLED);
    if (!*http_enabled)
        return;
    int *http_port = (int *) config_get(CONF_HTTP_PORT);

    client_length = (socklen_t) sizeof(http.cli_addr);

    http.running = true;

    http.srv_addr.sin_family = AF_INET;
    http.srv_addr.sin_addr.s_addr = INADDR_ANY;
    http.srv_addr.sin_port = htons(*http_port);    

    http.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (http.socket < 0)
        app_exit("[ERROR!] Could not create socket (http_init).", 1);

    if (bind(http.socket, (struct sockaddr *) &http.srv_addr, sizeof(http.srv_addr)) < 0)
        app_exit("[ERROR!] Could not bind socket to address (http_init).", 1); 

    listen(http.socket, DEFAULT_HTTP_MAX_CONNS);    

    http.thread = pthread_create(&(http.thread), NULL, http_main, NULL);
}

void http_halt()
{
    http.running = false;
    int error = pthread_join(http.thread, NULL);
    if (error)
        log_event("[ERROR!] Could not rejoin from robot thread.");
}

static void *http_main(void *arg)
{
    struct timespec time, last_time;
    double tick, diff, max_tick;

    int last_socket = -1;
    
    HTTPRequest http_request;
    HTTPResponse http_response;

    //tmp
    char *response_buffer = "HTTP/1.1 200 OK\r\nDate: Wed, May 23 2017 10:38:15 EST\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length:30\r\n\r\n{ \"an_object\": \"set_to_this\" }\r\n\r\n";

    clock_gettime(CLOCK_MONOTONIC, &last_time);
    while (http.running)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        diff = utils_timediff(time, last_time);
        last_time = time;

        tick += diff;
        if (tick < max_tick)
            continue;
        tick = 0.0;

        last_socket = accept(http.socket, (struct sockaddr *) &(http.cli_addr), &client_length);
        if (last_socket < 0) 
            continue;

        http_server_log_connection();

        bzero(http.buffer, (size_t) DEFAULT_HTTP_MAX_BUFFER);

        if (read(last_socket, http.buffer, DEFAULT_HTTP_MAX_BUFFER - 1) < 0)
            app_exit("[ERROR!] Could not read from last_socket (http_main).", 1);

        //tmp
        printf("%s", http.buffer);

        if (write(last_socket, response_buffer, DEFAULT_HTTP_RESPONSE_SIZE) < 0)
            app_exit("[ERROR!] Could not send return message to socket (http_main).", 1);

        close(last_socket);
    }

    close(http.socket);

    return NULL;
}

static void http_server_log_connection()
{
    char log_connection_msg[128];
    char client_ip_str[INET6_ADDRSTRLEN];    
    
    inet_ntop(AF_INET, (struct sockaddr_in *) &(http.cli_addr.sin_addr), client_ip_str, sizeof(client_ip_str));
    snprintf(log_connection_msg, 127, "[HTTP] Connecting to: %s", client_ip_str);    

    console_event(log_connection_msg);
    //log_event(log_connection_msg);
}

#endif