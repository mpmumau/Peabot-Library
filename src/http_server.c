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
#include "http_request_handler.h"

/* Header */
#include "http_server.h"

/* Forward decs */
static void *http_main(void *arg);
static void http_server_ipstr(char *str, int len);
static void http_server_log_connect(char *ipaddr);
static void http_server_log_http_request(HTTPRequest *http_request, int buff_size, char *ipaddr);

static bool running = true;
static int error;
static pthread_t http_server_thread;
static pthread_attr_t detached_thread_attr;

void http_init()
{  
    bool *http_enabled = (bool *) config_get(CONF_HTTP_ENABLED);
    if (!*http_enabled)
        return;

    pthread_attr_init(&detached_thread_attr);
    pthread_attr_setdetachstate(&detached_thread_attr, PTHREAD_CREATE_DETACHED);    

    error = pthread_create(&http_server_thread, &detached_thread_attr, http_main, NULL);
    if (error)
        APP_ERROR("Could not initialize HTTP thread.", error);    
}

void http_halt()
{
    running = false;
    close(http.socket);
    pthread_attr_destroy(&http_server_thread_attr);
}

static void *http_main(void *arg)
{
    prctl(PR_SET_NAME, "PEABOT_HTTP\0", NULL, NULL, NULL);

    unsigned short *http_port = (unsigned short *) config_get(CONF_HTTP_PORT);

    HTTPServer http;
    http.srv_addr.sin_family        = AF_INET;
    http.srv_addr.sin_addr.s_addr   = INADDR_ANY;
    http.srv_addr.sin_port          = htons(*http_port); 
    http.client_len                 = sizeof(http.cli_addr); 

    int last_socket;
    char ip_addr[INET6_ADDRSTRLEN];

    HTTPRequest *http_request;
    HTTPRequestData *request_data;

    pthread_t last_request_thread;

    http.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (http.socket < 0)
        APP_ERROR("Could not create socket.", errno);

    if (bind(http.socket, (struct sockaddr *) &(http.srv_addr), sizeof(http.srv_addr)) < 0)
        APP_ERROR("Could not bind socket to address.", errno);

    if (listen(http.socket, HTTP_SERVER_MAX_CONNS) < 0)    
        APP_ERROR("Could not listen on socket.", errno);  
   
    while (running)
    {
        last_socket = accept(http.socket, (struct sockaddr *) &(http.cli_addr), (socklen_t *) &(http.client_len));
        if (last_socket < 0) 
            continue;

        http_server_ipstr(ip_addr, sizeof(ip_addr));
        http_server_log_connect(ip_addr);

        memset(http.buffer, '\0', sizeof(http.buffer));   
        read(last_socket, http.buffer, sizeof(http.buffer) - 1);

        http_request = calloc(1, sizeof(HTTPRequest));
        if (!http_request)
            APP_ERROR("Could not allocate memory.", 1);  

        httpreq_reset_request(http_request);   
        httpreq_parse(http_request, ip_addr, http.buffer, sizeof(http.buffer));

        http_server_log_http_request(http_request, strlen(http.buffer), ip_addr);

        request_data = calloc(1, sizeof(HTTPRequestThreadData)); 
        if (!request_data)
            APP_ERROR("Could not allocate memory.", 1);  

        request_data->http_request = http_request;
        request_data->socket_fd = last_socket;

        pthread_create(&last_request_thread, &detached_thread_attr, httprhnd_handle_request, (void *) request_data);
    }

    close(http.socket);

    pthread_exit(NULL);

    return NULL;
}

static void http_server_ipstr(char *str, int len)
{
    inet_ntop(AF_INET, (struct sockaddr_in *) &(http.cli_addr.sin_addr), str, len);
}

static void http_server_log_connect(const char *ipaddr)
{
    char log_connection_msg[128];
    snprintf(log_connection_msg, sizeof(log_connection_msg) - 1, "[HTTP] Incoming request from: %s", ipaddr);   
    log_event(log_connection_msg);  
}

static void http_server_log_http_request(HTTPRequest *http_request, int buff_size, char *ipaddr)
{
    const char *request_type = httpreq_get_methodstr(http_request);

    char log_message[256];
    snprintf(log_message, sizeof(log_message)-1, "[HTTP] Request from %s is %s [%.2f %s]",
        ipaddr,
        request_type,
        buff_size < 1024 ? (double) buff_size : (double) (buff_size / 1024.0),
        buff_size < 1024 ? "bytes" : "kb" );
    log_event(log_message);
}

#endif