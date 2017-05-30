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
    prctl(PR_SET_NAME, "PEABOT_HTTP\0", NULL, NULL, NULL);

    http.running = true;
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

    fd_set socket_fd_set;
    int socket_select_result, iof = -1;
    struct timeval timeout;    
    timeout.tv_sec = 3;
    timeout.tv_usec = 0; 

    FD_ZERO(&socket_fd_set);
    FD_SET(http.socket, &socket_fd_set);   

    listen(http.socket, HTTP_SERVER_MAX_CONNS);    

    while (http.running)
    {
        socket_select_result = select(http.socket + 1, &socket_fd_set, NULL, NULL, &timeout);
        if (socket_select_result < 0)
        {
            printf("timeout\n");
            continue;
        }
        else if (socket_select_result > 0 && FD_ISSET(http.socket, &socket_fd_set))
        {
          if ((iof = fcntl(http.socket, F_GETFL, 0)) != -1)
             fcntl(http.socket, F_SETFL, iof | O_NONBLOCK);
        }

        last_socket = accept(http.socket, (struct sockaddr *) &(http.cli_addr), (socklen_t *) &client_length);
        if (last_socket < 0) 
            continue;

        http_server_ipstr(ip_addr, sizeof(ip_addr));
        http_server_log_connect(ip_addr);

        memset(http.buffer, '\0', sizeof(http.buffer));   
        read(last_socket, http.buffer, sizeof(http.buffer));

        http_request = calloc(1, sizeof(HTTPRequest));
        httpreq_reset_request(http_request);   
        httpreq_parse(http_request, ip_addr, http.buffer, sizeof(http.buffer));

        request_thread_data = calloc(1, sizeof(HTTPRequestThreadData));        
        request_thread_data->http_request = http_request;
        request_thread_data->socket_fd = last_socket;

        // dbg
        //printf("[HTTP RAW REQUEST]\n%s\n", http.buffer);

        pthread_create(&last_request_thread, &request_thread_attr, httprhnd_handle_request, (void *) request_thread_data);
    }

    pthread_attr_destroy(&request_thread_attr);
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