#ifndef CONTROLLER_EVENT_DEF
#define CONTROLLER_EVENT_DEF

/*
 File:          controller_event.c
 Description:   Controller functions for handling event objects
 Created:       May 28, 2017
 Author:        Matt Mumau
 */

/* Header */
#include "controller_event.h"

void cntlevent_walk(HTTPRequest *http_request, void *model_data)
{
    printf("WALK\n");
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