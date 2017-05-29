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

/* Libraries */
#include "cJSON.h"

/* Application includes */
#include "main.h"
#include "http_request.h"
#include "events.h"

/* Header */
#include "controller_event.h"

void cntlevent_walk(HTTPRequest *http_request, void *model_data)
{
    if (model_data == NULL)
        return;

    cJSON *json_p = (cJSON *) model_data;

    EventWalkData *event_walk_data = calloc(1, sizeof(EventWalkData));
    if (!event_walk_data)
        app_exit("[ERROR!] Could not allocate memory for event_walk_data (cntlevent_walk).", 1);

    cJSON *cycles_jp = cJSON_GetObjectItem(json_p, "cycles");
    if (!cycles_jp || !cJSON_IsNumber(cycles_jp))
        return;
    event_walk_data->cycles = (int) cycles_jp->valuedouble;
    

    cJSON *duration_jp = cJSON_GetObjectItem(json_p, "duration");
    if (!duration_jp || !cJSON_IsNumber(duration_jp))
        return;
    event_walk_data->duration = duration_jp->valuedouble;

    event_add(EVENT_WALK, (void *) event_walk_data);
}

void cntlevent_turn(HTTPRequest *http_request, void *model_data)
{
    printf("TURN\n");    
}

void cntlevent_elevate(HTTPRequest *http_request, void *model_data)
{
    printf("ELEVATE\n");    
}

void cntlevent_extend(HTTPRequest *http_request, void *model_data)
{
    printf("EXTEND\n");    
}

void cntlevent_delay(HTTPRequest *http_request, void *model_data)
{
    printf("DELAY\n");    
}

#endif