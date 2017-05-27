#ifndef HTTP_REQUEST_H_DEF
#define HTTP_REQUEST_H_DEF

/*
 File:          http_request.h
 Description:   Object for parsing HTTP requests.
 Created:       May 23, 2017
 Author:        Matt Mumau
 */

#define HTTP_BADREQUEST 0
#define HTTP_POST 1
#define HTTP_GET 2
#define HTTP_UPDATE 3
#define HTTP_DELETE 4

/* System includes */
#include <netinet/in.h>

/* Application includes */
#include "http_server.h"

typedef char HTTPRequestLine[DEFAULT_HTTP_LINE_LEN];

typedef struct HTTPRequest {
    char ip_addr[INET6_ADDRSTRLEN];
    unsigned int total_lines;
    int method;
    char uri[DEFAULT_HTTP_URI_LEN];
    char data[DEFAULT_HTTP_MAX_BUFFER - ((DEFAULT_HTTP_MAX_HEADERS + 1) * DEFAULT_HTTP_LINE_LEN)];
    bool v11;
} HTTPRequest;

void http_request_parse(HTTPRequest *http_request, char *raw, int buff_size);

#endif