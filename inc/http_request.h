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
#define HTTP_PUT 3
#define HTTP_DELETE 4

#define HTTP_CONTENT_TYPE_JSON 1

#define HTTP_REQ_BUFFER_LEN (256 * 1024) //256kb
#define HTTP_REQ_LINE_LEN 256
#define HTTP_REQ_MAX_HEADERS 31
#define HTTP_REQ_BODY_LEN (HTTP_REQ_BUFFER_LEN - ((HTTP_REQ_MAX_HEADERS + 1) * HTTP_REQ_LINE_LEN))

/* System includes */
#include <netinet/in.h>

typedef char HTTPRequestLine[HTTP_REQ_LINE_LEN];

typedef struct HTTPRequest {
    char    ip_addr[INET6_ADDRSTRLEN];
    bool    v11;
    int     method;
    char    uri[HTTP_REQ_LINE_LEN];
    int     hdr_content_type;
    char    hdr_user_agent[HTTP_REQ_LINE_LEN];
    bool    hdr_keep_alive;
    int     body_len;
    int     body_len_actual;
    char    body[HTTP_REQ_BODY_LEN];
} HTTPRequest;

void httpreq_reset_request(HTTPRequest *request);

void httpreq_parse(HTTPRequest *http_request, char *ip_addr, char *raw, int buff_size);

void httpreq_print(HTTPRequest *http_request);

#endif