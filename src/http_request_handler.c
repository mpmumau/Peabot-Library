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

void httprhnd_handle_request(HTTPRequest *http_request, int last_socket)
{
    printf("\nHandling request...\n");

    //char response_buffer[DEFAULT_HTTP_RESPONSE_SIZE] = "HTTP/1.1 200 OK\r\nDate: Wed, May 27 2017 12:49:15 EST\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: content-type\r\nContent-Length:30\r\n\r\n{ \"an_object\": \"set_to_this\" }\r\n\r\n";

    switch (http_request->method)
    {
        case HTTP_METHOD_GET:
            printf("\n[GET REQUEST DETECTED]\n");
            break;
        case HTTP_METHOD_POST:
            printf("\n[POST REQUEST DETECTED]\n");
            break;
        case HTTP_METHOD_PUT:
            printf("\n[PUT REQUEST DETECTED]\n");
            break;
        case HTTP_METHOD_DELETE:
            printf("\n[DELETE REQUEST DETECTED]\n");
            break;
        case HTTP_METHOD_OPTIONS:
            printf("\n[OPTIONS REQUEST DETECTED]\n");
            break;    
        default:
            printf("\n[BAD REQUEST DETECTED]\n");
            break;                       
    }
}

#endif