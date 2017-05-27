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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

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
static void http_server_ipstr(char *str, int str_size);
static void http_server_log_connect(char *ipaddr);

static HTTPServer http;

void http_init()
{  
    bool *http_enabled = (bool *) config_get(CONF_HTTP_ENABLED);
    if (!*http_enabled)
        return;
    http.thread = pthread_create(&(http.thread), NULL, http_main, NULL);
}

void http_halt()
{
    http.running = false;
    int error = pthread_join(http.thread, NULL);
    if (error)
        log_event("[ERROR!] Could not rejoin from HTTP thread.");
}

static void *http_main(void *arg)
{
    http.running = true;

    int *http_port = (int *) config_get(CONF_HTTP_PORT);

    http.srv_addr.sin_family = AF_INET;
    http.srv_addr.sin_addr.s_addr = INADDR_ANY;
    http.srv_addr.sin_port = htons(*http_port);      

    int last_socket = -1;

    HTTPRequest http_request;
    //HTTPResponse http_response;

    int client_length = sizeof(http.cli_addr);    

    char ip_addr[INET6_ADDRSTRLEN];

    http.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (http.socket < 0)
        app_exit("[ERROR!] Could not create socket (http_init).", 1);

    if (bind(http.socket, (struct sockaddr *) &(http.srv_addr), sizeof(http.srv_addr)) < 0)
        app_exit("[ERROR!] Could not bind socket to address (http_init).", 1); 

    listen(http.socket, DEFAULT_HTTP_MAX_CONNS);    

    //tmp - note this must be an array, not a pointer
    char response_buffer[DEFAULT_HTTP_RESPONSE_SIZE] = "HTTP/1.1 200 OK\r\nDate: Wed, May 27 2017 12:49:15 EST\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length:30\r\n\r\n{ \"an_object\": \"set_to_this\" }\r\n\r\n";

    struct timespec time, last_time;
    double tick, diff, max_tick;
    max_tick = 1.0;
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

        memset(http.buffer, '\0', DEFAULT_HTTP_MAX_BUFFER);

        last_socket = accept(http.socket, (struct sockaddr *) &(http.cli_addr), (socklen_t *) &client_length);
        if (last_socket < 0) 
            continue;

        http_server_ipstr(ip_addr, sizeof(ip_addr));
        http_server_log_connect(ip_addr);

        read(last_socket, http.buffer, DEFAULT_HTTP_MAX_BUFFER);
        http_request_parse(&http_request, http.buffer, sizeof(http.buffer));

        printf("request method: %d\n", http_request.method);

        write(last_socket, (void *) response_buffer, DEFAULT_HTTP_RESPONSE_SIZE);
        fsync(last_socket);

        close(last_socket);
    }

    close(http.socket);

    return NULL;
}

static void http_server_ipstr(char *str, int str_size)
{
    char client_ip_str[INET6_ADDRSTRLEN];    
    inet_ntop(AF_INET, (struct sockaddr_in *) &(http.cli_addr.sin_addr), client_ip_str, sizeof(client_ip_str));

    for (int i = 0; i < str_size; i++)
    {
        if (INET6_ADDRSTRLEN < (i + 1))
            break;
        str[i] = client_ip_str[i];
    }
}

static void http_server_log_connect(char *ipaddr)
{
    char log_connection_msg[128];
    snprintf(log_connection_msg, 127, "[HTTP] Connecting to: %s", ipaddr);   
    log_event(log_connection_msg);  
}

#endif