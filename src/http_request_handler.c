#ifndef HTTP_REQUEST_HANDLER_DEF
#define HTTP_REQUEST_HANDLER_DEF

/*
 File:          http_request_handler.c
 Description:   Implementation of handler functions for HTTP requests.
 Created:       May 28, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/* Application includes */
#include "http_request.h"
#include "string_utils.h"
#include "events.h"
#include "http_server.h"

/* Header */
#include "http_request_handler.h"

/* Forward decs */
static void httprhnd_handle_get(HTTPRequest *http_request);
static void httprhnd_handle_post(HTTPRequest *http_request);
static void httprhnd_handle_put(HTTPRequest *http_request);
static void httprhnd_handle_delete(HTTPRequest *http_request);
static void httprhnd_handle_options(HTTPRequest *http_request);
static int httprhnd_get_model(char *model_str);

void *httprhnd_handle_request(void *data)
{
    HTTPRequestThreadData *request_thread_data = (HTTPRequestThreadData *) data;
    HTTPRequest *http_request = request_thread_data->http_request;
    int socket_fd = request_thread_data->socket_fd;

    printf("\nHandling request...\n");

    //char response_buffer[DEFAULT_HTTP_RESPONSE_SIZE] = "HTTP/1.1 200 OK\r\nDate: Wed, May 27 2017 12:49:15 EST\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: content-type\r\nContent-Length:30\r\n\r\n{ \"an_object\": \"set_to_this\" }\r\n\r\n";

    char uri_cpy[sizeof(http_request->uri)];
    str_clearcopy(uri_cpy, http_request->uri, sizeof(uri_cpy));

    // printf("[URI] %s\n", uri_cpy);

    char *uri_p;
    if (uri_cpy[0] == '/')
        uri_p = &(uri_cpy[1]);

    // char *model_name = strtok(uri_p, "/");
    // char *controller_name = strtok(NULL, "?");
    // char *query_string = strtok(NULL, "\0");

    //int model = httprhnd_get_model(model_name);

    // printf("[Model] %s [Controller] %s [Query] %s\n", model_name, controller_name, query_string);

    void (*request_cb)(HTTPRequest *http_request);
    request_cb = NULL;

    if (http_request->method == HTTP_METHOD_GET)
        request_cb = httprhnd_handle_get;

    if (http_request->method == HTTP_METHOD_POST)
        request_cb = httprhnd_handle_post; 

    if (http_request->method == HTTP_METHOD_PUT)
        request_cb = httprhnd_handle_put;

    if (http_request->method == HTTP_METHOD_DELETE)
        request_cb = httprhnd_handle_delete;           

    if (http_request->method == HTTP_METHOD_OPTIONS)
        request_cb = httprhnd_handle_options;                   

    if (request_cb != NULL)
        (*request_cb)(http_request);

    close(socket_fd);
    // free(http_request);   
    // free(request_thread_data);

    pthread_exit(NULL);
}

static void httprhnd_handle_get(HTTPRequest *http_request)
{
    printf("\n[GET REQUEST DETECTED]\n");
    printf("URI: %s\n", http_request->uri);
}

static void httprhnd_handle_post(HTTPRequest *http_request)
{
    printf("\n[POST REQUEST DETECTED]\n");

    // tmp
    EventElevateData *elevate_data = calloc(1, sizeof(EventElevateData));
    elevate_data->duration = 5.0;
    elevate_data->reverse = false;
    event_add(EVENT_ELEVATE, (void *) elevate_data);     

    EventWalkData *walk_data = calloc(1, sizeof(EventWalkData));
    walk_data->cycles = 1;
    walk_data->duration = 2.0;   
    event_add(EVENT_WALK, (void *) walk_data);    
}

static void httprhnd_handle_put(HTTPRequest *http_request)
{
    printf("\n[PUT REQUEST DETECTED]\n");
}

static void httprhnd_handle_delete(HTTPRequest *http_request)
{
    printf("\n[DELETE REQUEST DETECTED]\n");
}

static void httprhnd_handle_options(HTTPRequest *http_request)
{
    printf("\n[OPTIONS REQUEST DETECTED]\n");
}

static int httprhnd_get_model(char *model_str)
{
    int model = MODEL_NONE;

    if (model_str == NULL)
        return model;

    if (strcmp(model_str, "event"))
        model = MODEL_EVENT;

    if (strcmp(model_str, "usd"))
        model = MODEL_USD;

    if (strcmp(model_str, "position"))
        model = MODEL_POSITION;

    return model;
}

#endif