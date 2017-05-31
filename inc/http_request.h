#ifndef HTTP_REQUEST_H_DEF
#define HTTP_REQUEST_H_DEF

/*
 File:          http_request.h
 Description:   Object for parsing HTTP requests.
 Created:       May 23, 2017
 Author:        Matt Mumau
 */

#define HTTP_METHOD_BADREQUEST 0
#define HTTP_METHOD_POST 1
#define HTTP_METHOD_GET 2
#define HTTP_METHOD_PUT 3
#define HTTP_METHOD_DELETE 4
#define HTTP_METHOD_OPTIONS 5

#define HTTP_CONTENT_TYPE_JSON 1

#define HTTP_REQ_BUFFER_LEN (256 * 1024) //256kb
#define HTTP_REQ_LINE_LEN 256
#define HTTP_REQ_MAX_HEADERS 31
#define HTTP_REQ_BODY_LEN (HTTP_REQ_BUFFER_LEN - ((HTTP_REQ_MAX_HEADERS + 1) * HTTP_REQ_LINE_LEN))

/* System includes */
#include <stdbool.h>
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
    bool    hdr_access_ctl_request_meth;
    int     body_len;
    int     body_len_actual;
    char    body[HTTP_REQ_BODY_LEN];
} HTTPRequest;

void httpreq_reset_request(HTTPRequest *request);

void httpreq_parse(HTTPRequest *http_request, char *ip_addr, char *raw, int buff_size);

const char *httpreq_get_methodstr(HTTPRequest *http_request);

#endif