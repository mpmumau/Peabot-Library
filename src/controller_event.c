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
    
    EventWalkData event_walk_data;
    event_walk_data.cycles = (unsigned short) cycles_jp->valuedouble;
    event_walk_data.duration = (double) duration_jp->valuedouble;
    event_walk_data.reverse = (bool) cJSON_IsTrue(reverse_jp);

    event_add(EVENT_WALK, (void *) &event_walk_data);
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

    cJSON *reverse_jp = cJSON_GetObjectItem(mvc_data->request_json, "reverse");
    if (!reverse_jp || !cJSON_IsBool(reverse_jp))
        return false;        
    
    EventTurnData event_turn_data;
    event_turn_data.cycles = (unsigned short) cycles_jp->valuedouble;
    event_turn_data.duration = (double) duration_jp->valuedouble;
    event_turn_data.reverse = (bool) cJSON_IsTrue(reverse_jp);

    event_add(EVENT_TURN, (void *) &event_turn_data);
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
    
    EventElevateData event_elevate_data;
    event_elevate_data.reverse = (bool) cJSON_IsTrue(reverse_jp);  
    event_elevate_data.duration = (double) duration_jp->valuedouble;

    event_add(EVENT_ELEVATE, (void *) &event_elevate_data); 
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
    
    EventExtendData event_extend_data;
    event_extend_data.reverse = (bool) cJSON_IsTrue(reverse_jp); 
    event_extend_data.duration = (double) duration_jp->valuedouble;

    event_add(EVENT_EXTEND, (void *) &event_extend_data); 
    return true;
}

bool cntlevent_delay(MVCData *mvc_data)
{
    cJSON *duration_jp = cJSON_GetObjectItem(mvc_data->request_json, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
        return false;

    double duration = (double) duration_jp->valuedouble;

    event_add(EVENT_EXTEND, (void *) &duration); 
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