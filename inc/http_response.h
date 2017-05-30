#ifndef HTTP_RESPONSE_H_DEF
#define HTTP_RESPONSE_H_DEF

/*
 File:          http_response.h
 Description:   Object representing HTTP responses.
 Created:       May 23, 2017
 Author:        Matt Mumau
 */

/* Application includes */
#include "stdbool.h"

#define HTTP_RES_LINE_LEN 256
#define HTTP_RES_MAX_HEADERS 31
#define HTTP_RES_MAX_LEN (1024*256)
#define HTTP_RES_BODY_LEN (HTTP_RES_MAX_LEN-((HTTP_RES_MAX_HEADERS+1)*HTTP_RES_LINE_LEN))

#define HTTP_RC_UNKNOWN -1
#define HTTP_RC_OK 200
#define HTTP_RC_BAD_REQUEST 400
#define HTTP_RC_FORBIDDEN 403
#define HTTP_RC_NOT_FOUND 404
#define HTTP_RC_INTERNAL_SERVER_ERROR 500

typedef struct HTTPResponse {
    int     code;
    char    body[HTTP_RES_BODY_LEN];
    char    content_type[128];
    bool    hdr_ac_allow_origin_all;
    bool    hdr_ac_allow_hdrs_content_type;
} HTTPResponse;

void http_response_init(HTTPResponse *http_response);

void http_response_tostring(HTTPResponse *http_response, char *response_str, size_t len);

#endif