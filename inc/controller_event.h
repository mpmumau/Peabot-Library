#ifndef CONTROLLER_EVENT_H_DEF
#define CONTROLLER_EVENT_H_DEF

/*
 File:          controller_event.h
 Description:   Controller function for HTTP event models.
 Created:       May 28, 2017
 Author:        Matt Mumau
 */

#include <stdbool.h>

bool cntlevent_walk(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data);

bool cntlevent_turn(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data);

bool cntlevent_elevate(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data);

bool cntlevent_extend(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data);

bool cntlevent_delay(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data);

#endif