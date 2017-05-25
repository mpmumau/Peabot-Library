#ifndef HTTP_REQUEST_DEF
#define HTTP_REQUEST_DEF

/*
 File:          http_request.h
 Description:   Object for parsing HTTP requests.
 Created:       May 23, 2017
 Author:        Matt Mumau
 */

/* System headers */
#include <stdio.h>
#include <string.h>

/* Application headers */
#include "string_utils.h"

/* Header */
#include "http_request.h"

/* Forward decs */
static void http_request_split_body(char *raw, char *first, char *second);

void http_request_parse(HTTPRequest *http_request, char *raw)
{
    char first[1024];
    char second[32768];
    http_request_split_body(char *raw, first, second);
}

static void http_request_split_body(char *raw, char *first, char *second)
{
    
}

#endif