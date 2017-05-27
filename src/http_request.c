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
#include <stdbool.h>

/* Application headers */
#include "http_server.h"
#include "string_utils.h"

/* Header */
#include "http_request.h"

/* Forward decs */
static void http_request_split_body(char *raw, char *first, char *second);

void http_request_parse(HTTPRequest *http_request, char *raw)
{
    if (http_request == NULL || raw == NULL)
        return;

    printf("raw: %s\n", raw);

    char buffer_cpy[DEFAULT_HTTP_MAX_BUFFER];
    memset(buffer_cpy, '\0', DEFAULT_HTTP_MAX_BUFFER);
    memcpy(buffer_cpy, raw, DEFAULT_HTTP_MAX_BUFFER - 1);
    buffer_cpy[DEFAULT_HTTP_MAX_BUFFER - 1] = '\0';

    printf("buffer_cpy: %s\n", buffer_cpy);


    bool add_extra_line = (DEFAULT_HTTP_MAX_BUFFER % DEFAULT_HTTP_LINE_LEN) > 0;
    int max_lines = (DEFAULT_HTTP_MAX_BUFFER - (DEFAULT_HTTP_MAX_BUFFER % DEFAULT_HTTP_LINE_LEN)) / DEFAULT_HTTP_LINE_LEN;
    
    if (add_extra_line)
        max_lines++;

    HTTPRequestLine lines[max_lines];

    int i;
    char *line_cursor = strtok(buffer_cpy, "\n");
    for (i = 0; i < max_lines; i++)
    {
        if (line_cursor == NULL)
            break;

        memset(&lines[i], '\0', DEFAULT_HTTP_LINE_LEN);
        memcpy(&lines[i], line_cursor, (size_t) DEFAULT_HTTP_LINE_LEN - 1);

        line_cursor = strtok(NULL, "\n");
    }

    int lines_added = i + 1;
    printf("Lines added: %d\n", lines_added);

    char *tmp_str;
    for (int p = 0; p < lines_added; p++)
    {
        printf( "Line [%d]: %s\n", p, lines[i]);
    }
}

static void http_request_split_body(char *raw, char *first, char *second)
{
    
}

#endif