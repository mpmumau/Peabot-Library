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
#include "controller_event.h"

/* Header */
#include "http_request_handler.h"

/* Forward decs */
static void httprhnd_handle_get(HTTPRequest *http_request, int model, char *controller);
static void httprhnd_handle_post(HTTPRequest *http_request, int model, char *controller);
static void httprhnd_handle_put(HTTPRequest *http_request, int model, char *controller);
static void httprhnd_handle_delete(HTTPRequest *http_request, int model, char *controller);
static void httprhnd_handle_options(HTTPRequest *http_request, int model, char *controller);
static int httprhnd_get_model(char *model_str);

void *httprhnd_handle_request(void *data)
{
    HTTPRequestThreadData *request_thread_data = (HTTPRequestThreadData *) data;
    HTTPRequest *http_request = request_thread_data->http_request;
    int socket_fd = request_thread_data->socket_fd;

    //char response_buffer[DEFAULT_HTTP_RESPONSE_SIZE] = "HTTP/1.1 200 OK\r\nDate: Wed, May 27 2017 12:49:15 EST\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: content-type\r\nContent-Length:30\r\n\r\n{ \"an_object\": \"set_to_this\" }\r\n\r\n";

    char uri_cpy[sizeof(http_request->uri)];
    str_clearcopy(uri_cpy, http_request->uri, sizeof(uri_cpy));

    char *uri_p;
    if (uri_cpy[0] == '/')
        uri_p = &(uri_cpy[1]);

    char *model_name = strtok(uri_p, "/");
    int model = httprhnd_get_model(model_name);
    char *controller_name = strtok(NULL, "?");

    void (*request_cb)(HTTPRequest *http_request, int model, char *controller);
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
        (*request_cb)(http_request, model, controller_name);

    close(socket_fd);
    free(http_request);   
    free(request_thread_data);

    pthread_exit(NULL);
}

static void httprhnd_handle_get(HTTPRequest *http_request, int model, char *controller)
{
    printf("\n[GET REQUEST DETECTED]\n");
}

static void httprhnd_handle_post(HTTPRequest *http_request, int model, char *controller)
{
    printf("\n[POST REQUEST DETECTED]\n");

    void (*post_cb)(HTTPRequest *http_request, void *model_data);
    post_cb = NULL;

    void *model_data = NULL;

    switch (model)
    {
        case MODEL_EVENT:
            if (strcmp(controller, "walk") == 0)
                post_cb = cntlevent_walk;
            if (strcmp(controller, "turn") == 0)
                post_cb = cntlevent_turn;
            if (strcmp(controller, "elevate") == 0)
                post_cb = cntlevent_elevate;
            if (strcmp(controller, "extend") == 0)
                post_cb = cntlevent_extend;            
            if (strcmp(controller, "delay") == 0)
                post_cb = cntlevent_extend;  
            break;
    }

    if (post_cb != NULL)
        (*post_cb)(http_request, model_data);
}

static void httprhnd_handle_put(HTTPRequest *http_request, int model, char *controller)
{
    printf("\n[PUT REQUEST DETECTED]\n");
}

static void httprhnd_handle_delete(HTTPRequest *http_request, int model, char *controller)
{
    printf("\n[DELETE REQUEST DETECTED]\n");
}

static void httprhnd_handle_options(HTTPRequest *http_request, int model, char *controller)
{
    printf("\n[OPTIONS REQUEST DETECTED]\n");
}

static int httprhnd_get_model(char *model_str)
{
    int model = MODEL_NONE;

    if (model_str == NULL)
        return model;

    if (strcmp(model_str, "event") == 0)
        model = MODEL_EVENT;

    if (strcmp(model_str, "usd") == 0)
        model = MODEL_USD;

    if (strcmp(model_str, "position") == 0)
        model = MODEL_POSITION;

    return model;
}

#endif