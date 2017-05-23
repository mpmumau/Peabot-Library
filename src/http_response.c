#ifndef HTTP_http_response_DEF
#define HTTP_http_response_DEF

/*
 File:          http_http_response.c
 Description:   Object representing HTTP http_responses.
 Created:       May 23, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdio.h>
#include <time.h>
#include <string.h>

/* Application includes */
#include "config_defaults.h"
#include "main.h"
#include "utils.h"

/* Header */
#include "http_response.h"

void http_response_set(HTTPResponse *http_response, int code, char *msg, char *body)
{
    http_response->code = code;
    http_response->msg = msg;
    http_response->body = body;
}

char *http_response_tobuffer(HTTPResponse *http_response)
{
    static char output[DEFAULT_HTTP_RESPONSE_SIZE]; // limits http_responses to max 32kb

    for (int u = 0; u < DEFAULT_HTTP_RESPONSE_SIZE; u++)
        output[u] = 0; 

    time_t current_time = time(NULL);
    char time_string[64];
    utils_mkresponsetime(current_time, time_string);

    char *http_line_fmt = "HTTP/1.0 %d %s\n";
    int http_line_size = snprintf(NULL, 128, http_line_fmt, http_response->code, http_response->msg) + 1;
    char http_line[http_line_size];
    snprintf(http_line, http_line_size, http_line_fmt, http_response->code, http_response->msg);

    char *date_line_fmt = "Date: %s\n";
    int date_line_size = snprintf(NULL, 128, date_line_fmt, time_string) + 1;
    char date_line[date_line_size];
    snprintf(date_line, date_line_size, date_line_fmt, time_string);

    char *content_type_line= "Content-Type: application/json\n";

    int max_body_size = DEFAULT_HTTP_RESPONSE_SIZE / 2;
    int body_size = 0;
    for ( ; body_size < max_body_size; body_size++)
    {
        if (http_response->body + body_size == '\0')
            break;
    }
    body_size++;

    char *content_length_line_fmt = "Content-Length: %d\n";
    int content_length_line_size = snprintf(NULL, 128, content_length_line_fmt, body_size);
    char content_length_line[content_length_line_size];
    snprintf(NULL, 128, content_length_line_fmt, body_size);

    char *buffer_format = "%s%s%s%s%s%s";
    snprintf(output, DEFAULT_HTTP_RESPONSE_SIZE, buffer_format, http_line, date_line, content_type_line, 
        content_length_line, "\r\n", http_response->body);

    return output;
}

#endif