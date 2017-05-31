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
#include <stdbool.h>

/* Application includes */
#include "utils.h"
#include "string_utils.h"

/* Header */
#include "http_response.h"

/* Forward decs */
static void http_response_appd_response_line(int code, char *output, size_t *len);
static void http_response_appd_date_line(char *output, size_t *len);
static void http_response_appd_content_type(char *content_type, char *output, size_t *len);
static void http_response_appd_ac_aoa(char *output, size_t *len);
static void http_response_appd_ac_ah(HTTPResponse *http_response, char *output, size_t *len);
static void http_response_appd_body(char *body, char *output, size_t *len);
static void http_response_set_msg_from_code(int code, char *msg, size_t len);

void http_response_init(HTTPResponse *http_response)
{
    http_response->code = HTTP_RC_UNKNOWN;
    memset(http_response->body, '\0', sizeof(http_response->body));
    http_response->hdr_ac_allow_origin_all = false;
    http_response->hdr_ac_allow_hdrs_content_type = false;
}

void http_response_tostring(HTTPResponse *http_response, char *response_str, size_t len)
{
    int full_len = len;
    char output[len];
    memset(output, '\0', sizeof(output));

    http_response_appd_response_line(http_response->code, output, &len);

    http_response_appd_date_line(output, &len);

    if (strlen(http_response->content_type))
    {
        printf("content-type: %s\n", http_response->content_type);
        http_response_appd_content_type(http_response->content_type, output, &len);
    }

    if (http_response->hdr_ac_allow_origin_all)
        http_response_appd_ac_aoa(output, &len);

    if (http_response->hdr_ac_allow_hdrs_content_type) // add OR clauses for other content types
        http_response_appd_ac_ah(http_response, output, &len);

    if (http_response->body[0] != '\0')
        http_response_appd_body(http_response->body, output, &len);

    str_clearcopy(response_str, output, full_len);
}

static void http_response_appd_response_line(int code, char *output, size_t *len)
{
    int added_len = 0;

    char response_msg[128];
    http_response_set_msg_from_code(code, response_msg, sizeof(response_msg));

    char response_line[HTTP_RES_LINE_LEN];
    added_len = snprintf(response_line, sizeof(response_line), "HTTP/1.1 %d %s\r\n", code, response_msg);
    strncat(output, response_line, *len - 1);    

    *len = *len - (added_len + 1);
}

static void http_response_appd_date_line(char *output, size_t *len)
{
    int added_len = 0;

    char response_time[128];
    utils_mkresponsetime(response_time, sizeof(response_time));    
    
    char date_line[HTTP_RES_LINE_LEN];
    added_len = snprintf(date_line, sizeof(date_line), "Date: %s\r\n", response_time);
    strncat(output, date_line, *len); 

    *len = *len - (added_len + 1);  
}

static void http_response_appd_content_type(char *content_type, char *output, size_t *len)
{
    int added_len = 0;

    char content_type_line[HTTP_RES_LINE_LEN];
    added_len = snprintf(content_type_line, sizeof(content_type_line), "content-type: %s\r\n", content_type);
    strncat(output, content_type_line, *len);

    *len = *len - (added_len + 1);  
}

static void http_response_appd_ac_aoa(char *output, size_t *len)
{
    char hdr_ac_aoa[] = "Access-Control-Allow-Origin: *\r\n";
    strncat(output, hdr_ac_aoa, *len);   

    *len = *len - sizeof(hdr_ac_aoa);  
}

static void http_response_appd_ac_ah(HTTPResponse *http_response, char *output, size_t *len)
{
    char hdr_ac_ah[HTTP_RES_LINE_LEN];
    strncat(hdr_ac_ah, "Access-Control-Allow-Headers:", sizeof(hdr_ac_ah));

    if (http_response->hdr_ac_allow_hdrs_content_type)
        strncat(hdr_ac_ah, " content-type", sizeof(hdr_ac_ah));

    strncat(hdr_ac_ah, "\r\n", sizeof(hdr_ac_ah));

    int added_len = strlen(hdr_ac_ah);

    strncat(output, hdr_ac_ah, *len);   

    *len = *len - (added_len + 1); 
}

static void http_response_appd_body(char *body, char *output, size_t *len)
{
    if (body == NULL || body[0] == '\0')
        return;

    strncat(output, "\r\n", 2);

    char body_str[HTTP_RES_BODY_LEN];
    memset(body_str, '\0', sizeof(body_str));
    memcpy(body_str, body, sizeof(body_str) - 1);

    strncat(output, body_str, *len);
}

static void http_response_set_msg_from_code(int code, char *msg, size_t len)
{
    char *tmp_msg;

    switch (code)
    {
        case HTTP_RC_UNKNOWN:
            tmp_msg = "Unknown";
            break;
        case HTTP_RC_OK: 
            tmp_msg = "OK";
            break;
        case HTTP_RC_BAD_REQUEST:
            tmp_msg = "Bad Request";
            break;
        case HTTP_RC_FORBIDDEN: 
            tmp_msg = "Forbidden";
            break;
        case HTTP_RC_NOT_FOUND:
            tmp_msg = "Not Found";
            break;
        case HTTP_RC_INTERNAL_SERVER_ERROR:
            tmp_msg = "Internal Server Error";
            break;
        default:
            tmp_msg = "Response Code Error";
            break;
    }

    str_clearcopy(msg, tmp_msg, len);
}

#endif