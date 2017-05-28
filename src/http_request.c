#ifndef HTTP_REQUEST_DEF
#define HTTP_REQUEST_DEF

/*
 File:          http_request.h
 Description:   Object for parsing HTTP requests.
 Created:       May 23, 2017
 Author:        Matt Mumau
 */

/* System headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Application headers */
#include "http_server.h"
#include "string_utils.h"

/* Header */
#include "http_request.h"

/* Forward decs */
static int httpreq_split_header(HTTPRequestLine *lines, size_t lines_len, char *raw, size_t raw_len);
static void httpreq_handle_header(HTTPRequest *http_request, HTTPRequestLine *line, int line_len);
static void httpreq_handle_request_line(HTTPRequest *http_request, HTTPRequestLine *line, int line_len);
static int httpreq_copy_buffer(char *dest, char *src, size_t size);
static int httpreq_copy_body(char *dest, char *src, size_t size);
static int httpreq_copy_header(char *dest, char *src, size_t size);
static int httpreq_parse_content_type(char *val);


void httpreq_parse(HTTPRequest *http_request, char *raw, int buff_size)
{
    if (http_request == NULL || raw == NULL)
        return;

    char buffer[HTTP_REQ_BUFFER_LEN];
    int buffer_len = httpreq_copy_buffer(buffer, raw, sizeof(buffer));

    char body[HTTP_REQ_BODY_LEN];
    int body_len = httpreq_copy_body(body, buffer, sizeof(body));
    http_request->body_len_actual = body_len;
    memset(http_request->body, '\0', sizeof(http_request->body));
    memcpy(http_request->body, body, sizeof(http_request->body)); 

    int MAX_HEADER_LEN = buffer_len - body_len;
    char header[MAX_HEADER_LEN];
    int header_len = httpreq_copy_header(header, buffer, sizeof(header));
    
    HTTPRequestLine header_lines[HTTP_REQ_MAX_HEADERS + 1];
    int lines_added = httpreq_split_header(header_lines, sizeof(header_lines), header, sizeof(header));

    httpreq_handle_request_line(http_request, &(header_lines[0]), sizeof(header_lines[0]));

    for (int i = 1; i < lines_added; i++)            
        httpreq_handle_header(http_request, &(header_lines[i]), sizeof(header_lines[i]));
}

static int httpreq_split_header(HTTPRequestLine *lines, size_t lines_len, char *raw, size_t raw_len)
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

static void httpreq_handle_header(HTTPRequest *http_request, HTTPRequestLine *line, int line_len)
{
    char line_cpy[line_len];
    memset(line_cpy, '\0', sizeof(line_cpy));
    memcpy(line_cpy, line, sizeof(line_cpy) - 1);

    char delim[1] = ":";
    char *key = strtok(line_cpy, delim);
    if (key == NULL)
        return;

    char *val = strtok(NULL, delim);
    if (val == NULL)
        return;

    printf("[HDR_KEY] %s [HDR_VAL] %s\n", key, val);

    if(strcmp(key, "User-Agent") == 0)
        str_clearcopy(http_request->hdr_user_agent, val, sizeof(http_request->hdr_user_agent));

    if (strcmp(key, "content-length") == 0)
        http_request->body_len = atoi(val);

    if (strcmp(key, "Content-Type") == 0)
        http_request->hdr_content_type = httpreq_parse_content_type(val);

    // etc...
}

static void httpreq_handle_request_line(HTTPRequest *http_request, HTTPRequestLine *line, int line_len)
{
    http_request->method = HTTP_BADREQUEST;
    memset(http_request->uri, '\0', sizeof(http_request->uri));
    http_request->v11 = false;

    char line_cpy[line_len];
    memset(line_cpy, '\0', sizeof(line_cpy));
    memcpy(line_cpy, line, sizeof(line_cpy) - 1);

    char *line_cursor;
    char delim[] = " ";

    line_cursor = strtok(line_cpy, delim);
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

    line_cursor = strtok(NULL, delim);
    if (line_cursor == NULL)
        return;

    memcpy(http_request->uri, line_cursor, sizeof(http_request->uri) - 1);

    line_cursor = strtok(NULL, delim);
    if (line_cursor == NULL)
        return;

    if (strcmp(line_cursor, "HTTP/1.1") == 0)
        http_request->v11 = true;
}

static int httpreq_copy_buffer(char *dest, char *src, size_t size)
{
    memset(dest, '\0', size);
    memcpy(dest, src, size - 1);
    dest[size - 1] = '\0';     

    return strlen(dest);
}

static int httpreq_copy_body(char *dest, char *src, size_t size)
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

static int httpreq_copy_header(char *dest, char *src, size_t size)
{
    memset(dest, '\0', size);
    memcpy(dest, &(src[0]), size - 1);

    return strlen(dest);
}

static int httpreq_get_max_lines()
{
    int max_lines = (HTTP_REQ_BUFFER_LEN - (HTTP_REQ_BUFFER_LEN % HTTP_REQ_LINE_LEN)) / HTTP_REQ_LINE_LEN;     
    bool add_extra_line = (HTTP_REQ_BUFFER_LEN % HTTP_REQ_LINE_LEN) > 0;
    if (add_extra_line)
        max_lines++;           
    return max_lines;
}

static int httpreq_parse_content_type(char *val)
{
    if (strcmp(val, "application/json") == 0)
        return HTTP_CONTENT_TYPE_JSON;
}

#endif