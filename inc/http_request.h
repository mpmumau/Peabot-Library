#ifndef HTTP_REQUEST_H_DEF
#define HTTP_REQUEST_H_DEF

/*
 File:          http_request.h
 Description:   Object for parsing HTTP requests.
 Created:       May 23, 2017
 Author:        Matt Mumau
 */

#define HTTP_POST 0
#define HTTP_GET 1
#define HTTP_UPDATE 2
#define HTTP_DELETE 3

typedef struct HTTPRequest {
    char *ip_addr;
    int method;
    char *uri;
    char *data;
} HTTPRequest;

void http_request_parse(HTTPRequest *http_request, char *raw);

#endif