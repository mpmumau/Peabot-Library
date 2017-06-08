#ifndef MVC_DATA_H_DEF
#define MVC_DATA_H_DEF

/*
 File:          mvc_data.h
 Description:   An object for wrapping all data for an MVC request.
 Created:       May 29, 2017
 Author:        Matt Mumau
 */

#define MODEL_NONE 0
#define MODEL_EVENT 1
#define MODEL_USD 2
#define MODEL_POSITION 3
#define MODEL_LOG 4

#define CONTROLLER_NONE 0
#define CONTROLLER_WALK 1
#define CONTROLLER_TURN 2
#define CONTROLLER_ELEVATE 3
#define CONTROLLER_EXTEND 4
#define CONTROLLER_RESET 5
#define CONTROLLER_DELAY 6
#define CONTROLLER_GET 7
#define CONTROLLER_HALT 8
#define CONTROLLER_STRAFE 9

/* Application includes */
#include "http_request.h"
#include "http_response.h"

typedef struct QueryStr {
    char name[64];
    char val[128];
} QueryStr;

typedef struct MVCData {
    HTTPRequest *http_request;
    HTTPResponse *http_response;
    int model;
    int controller;
    QueryStr query_str[256];
    cJSON *request_json;
    cJSON *response_json;
} MVCData;

void mvcdata_set(MVCData *mvc_data, HTTPRequest *http_request, HTTPResponse *http_response, char *model, char *controller, char *query);

void mvcdata_destroy(MVCData *mvc_data);

const char *mvcdata_get_modelstr(MVCData *mvc_data);

const char *mvcdata_get_controllerstr(MVCData *mvc_data);

#endif