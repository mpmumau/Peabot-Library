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
#include <sys/prctl.h>
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
#include <sys/select.h>
#include <fcntl.h>

/* Application includes */
#include "config.h"
#include "config_defaults.h"
#include "main.h"
#include "log.h"
#include "console.h"
#include "utils.h"
#include "http_request.h"
#include "http_response.h"
#include "http_request_handler.h"

/* Header */
#include "http_server.h"

/* Forward decs */
static void *http_main(void *arg);
static void http_server_ipstr(char *str, int str_size);
static void http_server_log_connect(char *ipaddr);
static void http_server_log_http_request(HTTPRequest *http_request, int buff_size, char *ipaddr);

static HTTPServer http;
static bool running;

void http_init()
{  
    bool *http_enabled = (bool *) config_get(CONF_HTTP_ENABLED);
    if (!*http_enabled)
        return;

    running = true;

    pthread_attr_t http_thread_attr;
    pthread_attr_init(&http_thread_attr);
    pthread_attr_setdetachstate(&http_thread_attr, PTHREAD_CREATE_DETACHED);

    http.thread = pthread_create(&(http.thread), &http_thread_attr, http_main, NULL);

    pthread_attr_destroy(&http_thread_attr);
}

void http_halt()
{
    running = false;
}

static void *http_main(void *arg)
{
    prctl(PR_SET_NAME, "PEABOT_HTTP\0", NULL, NULL, NULL);

    int *http_port = (int *) config_get(CONF_HTTP_PORT);

    http.srv_addr.sin_family = AF_INET;
    http.srv_addr.sin_addr.s_addr = INADDR_ANY;
    http.srv_addr.sin_port = htons(*http_port);      

    int client_length = sizeof(http.cli_addr); 
    int last_socket = -1;
    char ip_addr[INET6_ADDRSTRLEN];
    HTTPRequest *http_request;

    HTTPRequestThreadData *request_thread_data;
    pthread_t last_request_thread;
    pthread_attr_t request_thread_attr;
    pthread_attr_init(&request_thread_attr);
    pthread_attr_setdetachstate(&request_thread_attr, PTHREAD_CREATE_DETACHED);

    http.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (http.socket < 0)
        app_exit("[ERROR!] Could not create socket (http_init).", 1);

    if (bind(http.socket, (struct sockaddr *) &(http.srv_addr), sizeof(http.srv_addr)) < 0)
        app_exit("[ERROR!] Could not bind socket to address (http_init).", 1); 

    listen(http.socket, HTTP_SERVER_MAX_CONNS);    

    fd_set socket_fd_set;
    int socket_select_result;
    struct timeval timeout;
    int flags;   
   
    while (running)
    {
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;  

        FD_ZERO(&socket_fd_set);
        FD_SET(http.socket, &socket_fd_set);    

        socket_select_result = select(http.socket + 1, &socket_fd_set, NULL, NULL, &timeout);
        if (socket_select_result >= 0)
        {
            if (!FD_ISSET(http.socket, &socket_fd_set)) 
                continue;
        }

        flags = fcntl(http.socket, F_GETFL, 0);
        fcntl(http.socket, F_SETFL, flags | O_NONBLOCK);

        last_socket = accept(http.socket, (struct sockaddr *) &(http.cli_addr), (socklen_t *) &client_length);
        if (last_socket < 0) 
            continue;

        http_server_ipstr(ip_addr, sizeof(ip_addr));
        http_server_log_connect(ip_addr);

        memset(http.buffer, '\0', sizeof(http.buffer));   
        read(last_socket, http.buffer, sizeof(http.buffer) - 1);

        http_request = calloc(1, sizeof(HTTPRequest));
        httpreq_reset_request(http_request);   
        httpreq_parse(http_request, ip_addr, http.buffer, sizeof(http.buffer));

        http_server_log_http_request(http_request, strlen(http.buffer), ip_addr);

        request_thread_data = calloc(1, sizeof(HTTPRequestThreadData));        
        request_thread_data->http_request = http_request;
        request_thread_data->socket_fd = last_socket;

        pthread_create(&last_request_thread, &request_thread_attr, httprhnd_handle_request, (void *) request_thread_data);
    }

    pthread_attr_destroy(&request_thread_attr);
    close(http.socket);

    pthread_exit(NULL);

    return NULL;
}

static void http_server_ipstr(char *str, int str_size)
{
    inet_ntop(AF_INET, (struct sockaddr_in *) &(http.cli_addr.sin_addr), str, str_size);
}

static void http_server_log_connect(char *ipaddr)
{
    char log_connection_msg[128];
    snprintf(log_connection_msg, sizeof(log_connection_msg) - 1, "[HTTP] Incoming request from: %s", ipaddr);   
    log_event(log_connection_msg);  
}

static void http_server_log_http_request(HTTPRequest *http_request, int buff_size, char *ipaddr)
{
    const char *request_type = httpreq_get_methodstr(http_request);

    char log_message[256];
    snprintf(log_message, sizeof(log_message)-1, "[HTTP] Req[%s] is %s %.2f%s",
        ipaddr,
        request_type,
        buff_size < 1024 ? (double) buff_size : (double) (buff_size / 1024.0),
        buff_size < 1024 ? "b" : "kb" );
    log_event(log_message);
}

#endif