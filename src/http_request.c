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

    bool add_extra_line = (DEFAULT_HTTP_MAX_BUFFER % DEFAULT_HTTP_LINE_LEN) > 0;
    int max_lines = (DEFAULT_HTTP_MAX_BUFFER - (DEFAULT_HTTP_MAX_BUFFER % DEFAULT_HTTP_LINE_LEN)) / DEFAULT_HTTP_LINE_LEN;
    
    if (add_extra_line)
        max_lines++;
    printf ("Max lines: %d\n", max_lines);

    HTTPRequestLine lines[max_lines];

    char buffer_cpy[DEFAULT_HTTP_MAX_BUFFER];
    memset((void *) buffer_cpy, '\0', DEFAULT_HTTP_MAX_BUFFER);
    memcpy(raw, buffer_cpy, DEFAULT_HTTP_MAX_BUFFER);

    int i;
    char *line_cursor = strtok(buffer_cpy, "\r\n");
    for (i = 0; i < max_lines; i++)
    {
        if (line_cursor == NULL)
            break;

        memset((void *) &lines[i], '\0', DEFAULT_HTTP_LINE_LEN);
        memcpy(line_cursor, &lines[i], (size_t) DEFAULT_HTTP_LINE_LEN);

        line_cursor = strtok(NULL, "\r\n");
    }

    int lines_added = i + 1;
    printf("Lines added: %d\n", lines_added);

    char *tmp_str;
    for (int p = 0; p < lines_added; p++)
    {
        tmp_str = &lines[p];
        if(tmp_str[0] == '\0')
            continue;
        printf( "Line [%d]: %s\n", p, lines[i]);
    }
}

static void http_request_split_body(char *raw, char *first, char *second)
{
    
}

#endif