#ifndef HTTP_REQUEST_HANDLER_DEF
#define HTTP_REQUEST_HANDLER_DEF

/*
 File:          http_request_handler.c
 Description:   Implementation of handler functions for HTTP requests.
 Created:       May 28, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <sys/prctl.h>
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
#include "mvc_data.h"
#include "log.h"
#include "controller_usd.h"

/* Header */
#include "http_request_handler.h"

/* Forward decs */
static void httprhnd_response_global_conf(HTTPResponse *http_response);
static void httprhnd_conf_uri(HTTPRequest *http_request, char *model_name, char *controller_name, char *query_string);
static void httrhnd_send_response(MVCData *mvc_data, int socket_fd);
static void httprhnd_handle_get(MVCData *mvc_data);
static void httprhnd_handle_post(MVCData *mvc_data);
static void httprhnd_handle_put(MVCData *mvc_data);
static void httprhnd_handle_delete(MVCData *mvc_data);
static void httprhnd_handle_options(MVCData *mvc_data);
static void httprhnd_log_mvc_route(HTTPRequest *http_request, MVCData *mvc_data);

void *httprhnd_handle_request(void *data)
{
    prctl(PR_SET_NAME, "PEABOT_HTREQ\0", NULL, NULL, NULL);

    HTTPRequestThreadData *request_thread_data = (HTTPRequestThreadData *) data;
    
    int socket_fd               = request_thread_data->socket_fd;
    HTTPRequest *http_request   = request_thread_data->http_request;

    free(request_thread_data);

    HTTPResponse http_response;
    http_response_init(&http_response);
    httprhnd_response_global_conf(&http_response);

    char model[128];
    char controller[128];
    char query[512];
    httprhnd_conf_uri(http_request, model, controller, query);

    MVCData mvc_data;
    mvcdata_set(&mvc_data, http_request, &http_response, model, controller, query);
    httprhnd_log_mvc_route(http_request, &mvc_data);
    
    void (*request_cb)(MVCData *mvc_data);
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
        (*request_cb)(&mvc_data);

    httrhnd_send_response(&mvc_data, socket_fd);
    
    free(http_request);   
    mvcdata_destroy(&mvc_data);
    pthread_exit(NULL);
}

static void httprhnd_response_global_conf(HTTPResponse *http_response)
{
    http_response_init(http_response);
    
    http_response->hdr_ac_allow_origin_all = true;
    http_response->hdr_ac_allow_hdrs_content_type = true;     
}

static void httprhnd_conf_uri(HTTPRequest *http_request, char *model_name, char *controller_name, char *query_string)
{
    char *uri_p;
    char uri_cpy[sizeof(http_request->uri)];
    str_clearcopy(uri_cpy, http_request->uri, sizeof(uri_cpy));
   
    if (uri_cpy[0] == '/')
        uri_p = &(uri_cpy[1]);    

    char *model_name_p = strtok(uri_p, "/");
    if (model_name_p != NULL)
        str_clearcopy(model_name, model_name_p, 128);

    char *controller_name_p = strtok(NULL, "?");
    if (controller_name_p != NULL)
        str_clearcopy(controller_name, controller_name_p, 128);

    char *query_string_p = strtok(NULL, "\0");    
    if (query_string_p != NULL)
        str_clearcopy(query_string, query_string_p, 128);
}

static void httrhnd_send_response(MVCData *mvc_data, int socket_fd)
{
    if (mvc_data->http_response->code == HTTP_RC_OK)
    {
        char content_type[17] = "application/json"; 
        str_clearcopy(mvc_data->http_response->content_type, content_type, sizeof(mvc_data->http_response->content_type));

        char content[sizeof(mvc_data->http_response->body)];
        cJSON_PrintPreallocated(mvc_data->response_json, content, sizeof(content), true);
        str_clearcopy(mvc_data->http_response->body, content, sizeof(mvc_data->http_response->body));   

        printf("response_body: \n%s\n", mvc_data->http_response->body);
    } 

    char http_response_str[HTTP_RES_MAX_LEN];
    http_response_tostring(mvc_data->http_response, http_response_str, sizeof(http_response_str));

    write(socket_fd, http_response_str, strlen(http_response_str));
    close(socket_fd);    
}

static void httprhnd_handle_post(MVCData *mvc_data)
{
    bool (*post_cb)(MVCData *mvc_data);
    post_cb = NULL;

    switch (mvc_data->model)
    {
        case MODEL_EVENT:
            if (mvc_data->controller == CONTROLLER_WALK)
                post_cb = cntlevent_walk;
            if (mvc_data->controller == CONTROLLER_TURN)
                post_cb = cntlevent_turn;
            if (mvc_data->controller == CONTROLLER_ELEVATE)
                post_cb = cntlevent_elevate;
            if (mvc_data->controller == CONTROLLER_EXTEND)
                post_cb = cntlevent_extend;            
            if (mvc_data->controller == CONTROLLER_DELAY)
                post_cb = cntlevent_delay;  
            if (mvc_data->controller == CONTROLLER_RESET)
                post_cb = cntlevent_reset;  
            if (mvc_data->controller == CONTROLLER_HALT)
                post_cb = cntlevent_halt;  
            break;
        default:
            mvc_data->http_response->code = HTTP_RC_BAD_REQUEST;
    }

    bool success = false;
    if (post_cb != NULL)
        success = (*post_cb)(mvc_data);

    if (success)
    {
        cJSON_AddBoolToObject(mvc_data->response_json, "success", success);
        mvc_data->http_response->code = HTTP_RC_OK;
    }
    else
    {
        if (mvc_data->http_response->code != HTTP_RC_INTERNAL_SERVER_ERROR)
            mvc_data->http_response->code = HTTP_RC_BAD_REQUEST;
    }
}

static void httprhnd_handle_get(MVCData *mvc_data)
{
    bool (*get_cb)(MVCData *mvc_data);
    get_cb = NULL;

    switch (mvc_data->model)
    {
        case MODEL_USD:
            if (mvc_data->controller == CONTROLLER_GET)
                get_cb = cntlusd_getval;
            break;
    }

    bool success = false;
    if (get_cb != NULL)
    {
        success = (*get_cb)(mvc_data); 
    }

    if (success)
    {
        mvc_data->http_response->code = HTTP_RC_OK;
    }
    else
    {
        if (mvc_data->http_response->code != HTTP_RC_INTERNAL_SERVER_ERROR)
            mvc_data->http_response->code = HTTP_RC_BAD_REQUEST;
    }          
}

static void httprhnd_handle_put(MVCData *mvc_data)
{
    // nothing for now
}

static void httprhnd_handle_delete(MVCData *mvc_data)
{
    // nothing for now
}

static void httprhnd_handle_options(MVCData *mvc_data)
{
    // nothing for now
}

static void httprhnd_log_mvc_route(HTTPRequest *http_request, MVCData *mvc_data)
{
    const char *model_name = mvcdata_get_modelstr(mvc_data);
    const char *controller_name = mvcdata_get_controllerstr(mvc_data);

    char log_message[256];
    snprintf(log_message, sizeof(log_message) - 1, "[HTTP] Req[%s] [MDL] %s [CNTLR] %s",
        http_request->ip_addr,
        model_name,
        controller_name);

    log_event(log_message);
}

#endif