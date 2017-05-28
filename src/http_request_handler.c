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

/* Application includes */
#include "http_request.h"

/* Header */
#include "http_request_handler.h"

/* Forward decs */
static void httprhnd_handle_get(HTTPRequest *http_request);
static void httprhnd_handle_post(HTTPRequest *http_request);
static void httprhnd_handle_put(HTTPRequest *http_request);
static void httprhnd_handle_delete(HTTPRequest *http_request);
static void httprhnd_handle_options(HTTPRequest *http_request);

void httprhnd_handle_request(HTTPRequest *http_request)
{
    printf("\nHandling request...\n");

    //char response_buffer[DEFAULT_HTTP_RESPONSE_SIZE] = "HTTP/1.1 200 OK\r\nDate: Wed, May 27 2017 12:49:15 EST\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: content-type\r\nContent-Length:30\r\n\r\n{ \"an_object\": \"set_to_this\" }\r\n\r\n";

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
        request_cb = httprhnd_handle_delete;                   

    if (request_cb != NULL)
        (*request_cb)(http_request);
}

static void httprhnd_handle_get(HTTPRequest *http_request)
{
    printf("\n[GET REQUEST DETECTED]\n");
}

static void httprhnd_handle_post(HTTPRequest *http_request)
{
    printf("\n[POST REQUEST DETECTED]\n");
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

#endif