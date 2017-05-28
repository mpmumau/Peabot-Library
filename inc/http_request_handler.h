#ifndef HTTP_REQUEST_HANDLER_H_DEF
#define HTTP_REQUEST_HANDLER_H_DEF

/*
 File:          http_request_handler.h
 Description:   Handler functions for HTTP requests.
 Created:       May 28, 2017
 Author:        Matt Mumau
 */

#define MODEL_NONE 0
#define MODEL_EVENT 1
#define MODEL_USD 2
#define MODEL_POSITION 3

/* Application includes */
#include "http_request.h"

void httprhnd_handle_request(HTTPRequest *http_request, int socket_fd);

#endif