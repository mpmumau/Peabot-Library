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

/* Application includes */
#include "main.h"
#include "config.h"
#include "config_defaults.h"
#include "utils.h"
#include "http_request.h"
#include "http_response.h"

/* Header */
#include "http_server.h"

/* Forward decs */
static void *http_main(void *arg);

static HTTPServer http;

void http_init()
{  
    bool *http_enabled = (bool *) config_get(CONF_HTTP_ENABLED);
    int *http_port = (int *) config_get(CONF_HTTP_PORT);

    if (!*http_enabled)
        return;

    http.running = true;
    http.thread = pthread_create(&(http.thread), NULL, http_main, NULL);
    
    http.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (http.socket < 0)
        app_exit("[ERROR!] Could not create socket (http_init).", 1);

    http.srv_addr.sin_family = AF_INET;
    http.srv_addr.sin_addr.s_addr = INADDR_ANY;
    http.srv_addr.sin_port = htons(*http_port);

    if (bind(http.socket, (struct sockaddr *) &http.srv_addr, sizeof(http.srv_addr))
        < 0)
    {
        app_exit("[ERROR!] Could not bind socket to address (http_init).", 1); 
    }
}

void http_halt()
{
    http.running = false;
    pthread_join(http.thread, NULL);
}

static void *http_main(void *arg)
{
    struct timespec time, last_time;
    float tick = 0.0f, diff = 0.0f, max_tick = 0.25f;

    socklen_t client_length = (socklen_t) sizeof(http.cli_addr);
    int last_socket = -1;

    HTTPResponse http_response;
    char *response_buffer = "HTTP/1.0 200 OK\r\nDate: Wed, Mar 22 2019 12:14:15 GMT\r\nContent-Type: application/json\r\nContent-Length:27\r\n\r\n{'an_object':'set_to_this'}\n";

    while (http.running)
    {
        // Timing mechanism
        clock_gettime(CLOCK_MONOTONIC, &time);
        diff = utils_timediff(time, last_time);
        last_time = time;
        
        tick += diff;
        if (tick < max_tick)
            continue;

        tick = 0.0f;

        // Loop start 
        listen(http.socket, DEFAULT_HTTP_MAX_CONNS);

        last_socket = accept(http.socket, (struct sockaddr *) &(http.cli_addr), &client_length);
        if (last_socket < 0) 
          app_exit("[ERROR!] Problem occured with last_socket (http_main).", 1);

        bzero(http.buffer, (size_t) DEFAULT_HTTP_BUFFER_SIZE);

        if (read(last_socket, http.buffer, DEFAULT_HTTP_BUFFER_SIZE - 1) < 0)
            app_exit("[ERROR!] Could not read from last_socket (http_main).", 1);

        printf("\n------------------------------------------------------------------\n");
        printf("%s", http.buffer);
        printf("\n------------------------------------------------------------------\n");

        // http_response_set(&http_response, 200, "OK", "{ 'an_object': 'set_to_this' }");
        // response_buffer = http_response_tobuffer(&http_response);

        printf(response_buffer);

        if (write(last_socket, response_buffer, DEFAULT_HTTP_RESPONSE_SIZE) < 0)
            app_exit("[ERROR!] Could not send return message to socket (http_main).", 1);

        close(last_socket);
    }

    close(http.socket);

    return NULL;
}

#endif