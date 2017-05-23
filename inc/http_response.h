#ifndef HTTP_RESPONSE_H_DEF
#define HTTP_RESPONSE_H_DEF

/*
 File:          http_response.h
 Description:   Object representing HTTP responses.
 Created:       May 23, 2017
 Author:        Matt Mumau
 */

typedef struct HTTPResponse {
    int code;
    char *msg;
    char *body;
} HTTPResponse;

void http_response_set(HTTPResponse *http_response, int code, char *msg, char *body);

char *http_response_tobuffer(HTTPResponse *http_response);

#endif