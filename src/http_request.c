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
static int http_request_split_header_lines(HTTPRequestLine *lines, size_t lines_len, char *raw, size_t raw_len);
static void http_request_handle_header_lines(HTTPRequest *http_request, HTTPRequestLine *lines, int lines_len);
static void http_request_handle_request_line(HTTPRequest *http_request, HTTPRequestLine *line);
static int http_request_copy_buffer(char *dest, char *src, size_t size);
static int http_request_copy_body(char *dest, char *src, size_t size);
static int http_request_copy_header(char *dest, char *src, size_t size);


void http_request_parse(HTTPRequest *http_request, char *raw, int buff_size)
{
    if (http_request == NULL || raw == NULL)
        return;

    char buffer[HTTP_REQ_BUFFER_LEN];
    int buffer_len = http_request_copy_buffer(buffer, raw, sizeof(buffer));

    char body[HTTP_REQ_BODY_LEN];
    int body_len = http_request_copy_body(body, buffer, sizeof(body));
    memset(http_request->body, '\0', sizeof(http_request->body));
    memcpy(http_request->body, body, sizeof(http_request->body)); 

    int MAX_HEADER_LEN = buffer_len - body_len;
    char header[MAX_HEADER_LEN];
    int header_len = http_request_copy_header(header, buffer, sizeof(header));
    HTTPRequestLine header_lines[HTTP_REQ_MAX_HEADERS + 1];
    int lines_added = http_request_split_header_lines(header_lines, sizeof(header_lines), header, sizeof(header));

    printf("\n");
    for (int i = 0; i < lines_added; i++)
        printf("[L%d] %s\n", i, header_lines[i]);

    printf("[body]\n%s\n", http_request->body);
}

static int http_request_split_header_lines(HTTPRequestLine *lines, size_t lines_len, char *raw, size_t raw_len)
{
    char raw_cpy[raw_len];
    memset(raw_cpy, '\0', sizeof(raw_cpy));
    memcpy(raw_cpy, raw, sizeof(raw_cpy) - 1);

    int lines_added;
    char line_delim[2] = "\r\n";

    char *token = strtok(raw_cpy, line_delim);

    for (lines_added = 0; lines_added < lines_len; lines_added++)
    {
        if (token == NULL || token[0] == '\0' || token[0] == '\r')
            break;
        memset(&(lines[lines_added]), '\0', HTTP_REQ_LINE_LEN);
        memcpy(&(lines[lines_added]), token, HTTP_REQ_LINE_LEN - 1); 
        token = strtok(NULL, line_delim);
    }

    return lines_added;
}

static void http_request_handle_header_lines(HTTPRequest *http_request, HTTPRequestLine *lines, int lines_len)
{
    if (lines == NULL || lines_len <= 0)
        return;

    http_request_handle_request_line(http_request, &(lines[0]));

    if (lines_len == 1)
        return;

    for (int i = 1; i < lines_len; i++)
    {
        // todo: handle headers as neccessary
        printf("[H#%d]: %s\n", i, lines[i]);
    }
}

static void http_request_handle_request_line(HTTPRequest *http_request, HTTPRequestLine *line)
{
    if (line == NULL)
        return;

    char line_cpy[HTTP_REQ_LINE_LEN];
    memset(line_cpy, '\0', HTTP_REQ_LINE_LEN);
    memcpy(line_cpy, line, HTTP_REQ_LINE_LEN - 1);

    char *line_cursor;
    char delim[] = " ";
    
    http_request->method = HTTP_BADREQUEST;
    line_cursor = strtok((char *) line, delim);
    if (line_cursor == NULL)
        return;
    if (strcmp(line_cursor, "POST") == 0)
        http_request->method = HTTP_POST;    
    if (strcmp(line_cursor, "GET") == 0)
        http_request->method = HTTP_GET;
    if (strcmp(line_cursor, "PUT") == 0)
        http_request->method = HTTP_PUT;
    if (strcmp(line_cursor, "DELETE") == 0)
        http_request->method = HTTP_DELETE;

    memset(http_request->uri, '\0', HTTP_REQ_LINE_LEN);
    line_cursor = strtok(NULL, delim);
    if (line_cursor == NULL)
        return;
    memcpy(http_request->uri, line_cursor, HTTP_REQ_LINE_LEN);

    http_request->v11 = false;
    line_cursor = strtok(NULL, delim);
    if (line_cursor == NULL)
        return;

    if (strcmp(line_cursor, "HTTP/1.1") == 0)
        http_request->v11 = true;
}

static int http_request_copy_buffer(char *dest, char *src, size_t size)
{
    memset(dest, '\0', size);
    memcpy(dest, src, size - 1);
    dest[size - 1] = '\0';     

    return strlen(dest);
}

static int http_request_copy_body(char *dest, char *src, size_t size)
{
    char copy[size];
    memset(copy, '\0', size);
    memcpy(copy, src, size - 1);

    char *body_substr = strstr(copy, "\r\n\r\n");
    if (body_substr == NULL)
        return 0;

    memset(dest, '\0', size);
    memcpy(dest, &(body_substr[4]), size - 1);
   
    return strlen(dest);
}

static int http_request_copy_header(char *dest, char *src, size_t size)
{
    memset(dest, '\0', size);
    memcpy(dest, &(src[0]), size - 1);

    return strlen(dest);
}

static int http_request_get_max_lines()
{
    int max_lines = (HTTP_REQ_BUFFER_LEN - (HTTP_REQ_BUFFER_LEN % HTTP_REQ_LINE_LEN)) / HTTP_REQ_LINE_LEN;     
    bool add_extra_line = (HTTP_REQ_BUFFER_LEN % HTTP_REQ_LINE_LEN) > 0;
    if (add_extra_line)
        max_lines++;           
    return max_lines;
}

#endif