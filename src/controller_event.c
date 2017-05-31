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
#include "mvc_data.h"

/* Header */
#include "controller_event.h"

bool cntlevent_walk(MVCData *mvc_data)
{
    cJSON *cycles_jp = cJSON_GetObjectItem(mvc_data->request_json, "cycles");
    if (!cycles_jp || !cJSON_IsNumber(cycles_jp))
        return false;
    
    cJSON *duration_jp = cJSON_GetObjectItem(mvc_data->request_json, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
        return false;

    cJSON *reverse_jp = cJSON_GetObjectItem(mvc_data->request_json, "reverse");
    if (!reverse_jp || !cJSON_IsBool(reverse_jp))
        return false;    
    
    EventWalkData *event_walk_data = calloc(1, sizeof(EventWalkData));
    if (!event_walk_data)
    {
        mvc_data->http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        return false;
    }

    event_walk_data->cycles = (int) cycles_jp->valuedouble;
    event_walk_data->duration = (float) duration_jp->valuedouble;
    event_walk_data->reverse = reverse_jp->valuedouble == 0.0 ? true : false;

    printf("event_walk_data->cycles: %d\n", event_walk_data->cycles);
    printf("event_walk_data->duration: %f\n", event_walk_data->duration);
    printf("event_walk_data->reverse: %s\n", event_walk_data->reverse ? "true" : "false");

    event_add(EVENT_WALK, (void *) event_walk_data);
    return true;
}

bool cntlevent_turn(MVCData *mvc_data)
{    
    cJSON *cycles_jp = cJSON_GetObjectItem(mvc_data->request_json, "cycles");
    if (!cycles_jp || !cJSON_IsNumber(cycles_jp))
        return false;
        
    cJSON *duration_jp = cJSON_GetObjectItem(mvc_data->request_json, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
        return false;
    
    EventTurnData *event_turn_data = calloc(1, sizeof(EventTurnData));
    if (!event_turn_data)
    {
        mvc_data->http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        return false;
    }

    event_turn_data->cycles = (int) cycles_jp->valuedouble;
    event_turn_data->duration = (float) duration_jp->valuedouble;

    event_add(EVENT_TURN, (void *) event_turn_data);
    return true;
}

bool cntlevent_elevate(MVCData *mvc_data)
{
    cJSON *reverse_jp = cJSON_GetObjectItem(mvc_data->request_json, "reverse");
    if (!reverse_jp || !cJSON_IsBool(reverse_jp))
        return false;
      
    cJSON *duration_jp = cJSON_GetObjectItem(mvc_data->request_json, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
        return false;
    
    EventElevateData *event_elevate_data = calloc(1, sizeof(EventElevateData));
    if (!event_elevate_data)
    {
        mvc_data->http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        return false;
    }

    event_elevate_data->reverse = (bool) reverse_jp->valuedouble;  
    event_elevate_data->duration = (float) duration_jp->valuedouble;

    event_add(EVENT_ELEVATE, (void *) event_elevate_data); 
    return true;
}

bool cntlevent_extend(MVCData *mvc_data)
{
    cJSON *reverse_jp = cJSON_GetObjectItem(mvc_data->request_json, "reverse");
    if (!reverse_jp || !cJSON_IsBool(reverse_jp))
        return false;
       
    cJSON *duration_jp = cJSON_GetObjectItem(mvc_data->request_json, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
        return false;
    
    EventExtendData *event_extend_data = calloc(1, sizeof(EventExtendData));
    if (!event_extend_data)
    {
        mvc_data->http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        return false;
    }

    event_extend_data->reverse = (bool) reverse_jp->valuedouble; 
    event_extend_data->duration = (float) duration_jp->valuedouble;

    event_add(EVENT_EXTEND, (void *) event_extend_data); 
    return true;
}

bool cntlevent_delay(MVCData *mvc_data)
{
    cJSON *duration_jp = cJSON_GetObjectItem(mvc_data->request_json, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
        return false;

    float *duration = calloc(1, sizeof(float));
    if (!duration)
    {
        mvc_data->http_response->code = HTTP_RC_INTERNAL_SERVER_ERROR;
        return false;
    }

    *duration = (float) duration_jp->valuedouble; 

    event_add(EVENT_EXTEND, (void *) duration); 
    return true;
}

bool cntlevent_reset(MVCData *mvc_data)
{
    event_add(EVENT_RESET, (void *) NULL); 
    return true;
}

bool cntlevent_halt(MVCData *mvc_data)
{
    event_add(EVENT_HALT, (void *) NULL);
    return true;
}

#endif