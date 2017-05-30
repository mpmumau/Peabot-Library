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

/* Libraries */
#include "cJSON.h"

/* Application includes */
#include "http_server.h"
#include "http_request.h"
#include "http_response.h"
#include "controller_event.h"
#include "events.h"
#include "string_utils.h"

/* Header */
#include "http_request_handler.h"

/* Forward decs */
static void httprhnd_handle_get(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller);
static void httprhnd_handle_post(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller);
static void httprhnd_handle_put(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller);
static void httprhnd_handle_delete(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller);
static void httprhnd_handle_options(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller);
static int httprhnd_get_model(char *model_str);

void *httprhnd_handle_request(void *data)
{
    HTTPRequestThreadData *request_thread_data = (HTTPRequestThreadData *) data;
    
    int socket_fd               = request_thread_data->socket_fd;
    HTTPRequest *http_request   = request_thread_data->http_request;

    HTTPResponse http_response;
    http_response_init(&http_response);
    char http_response_str[HTTP_RES_MAX_LEN]; //...suspect; needed?    

    char uri_cpy[sizeof(http_request->uri)];
    str_clearcopy(uri_cpy, http_request->uri, sizeof(uri_cpy));
    char *uri_p;
    if (uri_cpy[0] == '/')
        uri_p = &(uri_cpy[1]);

    char *model_name = strtok(uri_p, "/");
    int model = httprhnd_get_model(model_name);
    char *controller_name = strtok(NULL, "?");

    void (*request_cb)(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller);
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
        (*request_cb)(http_request, &http_response, model, controller_name);

    http_response_tostring(&http_response, http_response_str, sizeof(http_response_str));

    printf("response_string: %s\n", http_response_str);
    
    write(socket_fd, http_response_str, strlen(http_response_str));
    close(socket_fd);
    
    free(http_request);   
    free(request_thread_data);
    pthread_exit(NULL);
}

static void httprhnd_handle_get(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller)
{
    printf("\n[GET REQUEST DETECTED]\n");
}

static void httprhnd_handle_post(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller)
{
    printf("\n[POST REQUEST DETECTED]\n");

    void (*post_cb)(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data);
    post_cb = NULL;

    cJSON *req_data_p = cJSON_Parse(http_request->body);
    cJSON *res_data_p = cJSON_CreateObject();

    cJSON_AddStringToObject(res_data_p, "method", "post");

    switch (model)
    {
        case MODEL_EVENT:
            cJSON_AddStringToObject(res_data_p, "model", "event");

            if (strcmp(controller, "walk") == 0)
            {
                cJSON_AddStringToObject(res_data_p, "controller", "walk");
                post_cb = cntlevent_walk;
            }
            if (strcmp(controller, "turn") == 0)
            {
                cJSON_AddStringToObject(res_data_p, "controller", "turn");
                post_cb = cntlevent_turn;
            }
            if (strcmp(controller, "elevate") == 0)
            {
                cJSON_AddStringToObject(res_data_p, "controller", "elevate");
                post_cb = cntlevent_elevate;
            }
            if (strcmp(controller, "extend") == 0)
            {
                cJSON_AddStringToObject(res_data_p, "controller", "extend");
                post_cb = cntlevent_extend;            
            }
            if (strcmp(controller, "delay") == 0)
            {
                cJSON_AddStringToObject(res_data_p, "controller", "delay");
                post_cb = cntlevent_delay;  
            }
            break;
    }

    if (post_cb != NULL)
        (*post_cb)(http_request, http_response, res_data_p, (void *) req_data_p);

    char tmp[sizeof(http_response->body)];
    cJSON_PrintPreallocated(res_data_p, tmp, sizeof(tmp), false);

    char content_type[17] = "application/json"; 
    str_clearcopy(http_response->content_type, content_type, sizeof(http_response->content_type));

    str_clearcopy(http_response->body, tmp, sizeof(http_response->body));

    cJSON_Delete(req_data_p);
    cJSON_Delete(res_data_p);
}

static void httprhnd_handle_put(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller)
{
    printf("\n[PUT REQUEST DETECTED]\n");
}

static void httprhnd_handle_delete(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller)
{
    printf("\n[DELETE REQUEST DETECTED]\n");
}

static void httprhnd_handle_options(HTTPRequest *http_request, HTTPResponse *http_response, int model, char *controller)
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