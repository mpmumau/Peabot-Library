#ifndef CONTROLLER_EVENT_DEF
#define CONTROLLER_EVENT_DEF

/*
 File:          controller_event.c
 Description:   Controller functions for handling event objects
 Created:       May 28, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Libraries */
#include "cJSON.h"

/* Application includes */
#include "main.h"
#include "http_request.h"
#include "events.h"
#include "http_response.h"

/* Header */
#include "controller_event.h"

void cntlevent_walk(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data)
{
    if (model_data == NULL)
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }

    cJSON *json_p = (cJSON *) model_data;

    cJSON *cycles_jp = cJSON_GetObjectItem(json_p, "cycles");
    if (!cycles_jp || !cJSON_IsNumber(cycles_jp))
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    
    cJSON *duration_jp = cJSON_GetObjectItem(json_p, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    
    EventWalkData *event_walk_data = calloc(1, sizeof(EventWalkData));
    if (!event_walk_data)
    {
        http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }

    event_walk_data->cycles = (int) cycles_jp->valuedouble;
    event_walk_data->duration = (float) duration_jp->valuedouble;

    event_add(EVENT_WALK, (void *) event_walk_data);

    cJSON_AddTrueToObject(resjs, "success");
}

void cntlevent_turn(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data)
{
    if (model_data == NULL)
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    cJSON *json_p = (cJSON *) model_data;
    
    cJSON *cycles_jp = cJSON_GetObjectItem(json_p, "cycles");
    if (!cycles_jp || !cJSON_IsNumber(cycles_jp))
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
        
    cJSON *duration_jp = cJSON_GetObjectItem(json_p, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    
    EventTurnData *event_turn_data = calloc(1, sizeof(EventTurnData));
    if (!event_turn_data)
    {
        http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    event_turn_data->cycles = (int) cycles_jp->valuedouble;
    event_turn_data->duration = (float) duration_jp->valuedouble;

    event_add(EVENT_TURN, (void *) event_turn_data);

    cJSON_AddTrueToObject(resjs, "success");
}

void cntlevent_elevate(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data)
{
    if (model_data == NULL)
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    cJSON *json_p = (cJSON *) model_data;

    cJSON *reverse_jp = cJSON_GetObjectItem(json_p, "reverse");
    if (!reverse_jp || !cJSON_IsBool(reverse_jp))
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
      
    cJSON *duration_jp = cJSON_GetObjectItem(json_p, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    
    EventElevateData *event_elevate_data = calloc(1, sizeof(EventElevateData));
    if (!event_elevate_data)
    {
        http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    event_elevate_data->reverse = (bool) reverse_jp->valuedouble;  
    event_elevate_data->duration = (float) duration_jp->valuedouble;

    event_add(EVENT_ELEVATE, (void *) event_elevate_data); 

    cJSON_AddTrueToObject(resjs, "success");
}

void cntlevent_extend(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data)
{
    if (model_data == NULL)
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    cJSON *json_p = (cJSON *) model_data;

    cJSON *reverse_jp = cJSON_GetObjectItem(json_p, "reverse");
    if (!reverse_jp || !cJSON_IsBool(reverse_jp))
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
       
    cJSON *duration_jp = cJSON_GetObjectItem(json_p, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    
    EventExtendData *event_extend_data = calloc(1, sizeof(EventExtendData));
    if (!event_extend_data)
    {
        http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    event_extend_data->reverse = (bool) reverse_jp->valuedouble; 
    event_extend_data->duration = (float) duration_jp->valuedouble;

    event_add(EVENT_EXTEND, (void *) event_extend_data); 

    cJSON_AddTrueToObject(resjs, "success");    

    printf("did cntlevent_extend\n");
}

void cntlevent_delay(HTTPRequest *http_request, HTTPResponse *http_response, cJSON *resjs, void *model_data)
{
    if (model_data == NULL)
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    cJSON *json_p = (cJSON *) model_data;

    cJSON *duration_jp = cJSON_GetObjectItem(json_p, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
    {
        http_response->code = HTTP_RC_BAD_REQUEST;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }
    float *duration = calloc(1, sizeof(float));
    if (!duration)
    {
        http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        cJSON_AddFalseToObject(resjs, "success");
        return;
    }

    *duration = (float) duration_jp->valuedouble; 

    event_add(EVENT_EXTEND, (void *) duration); 

    cJSON_AddTrueToObject(resjs, "success"); 
}

#endif