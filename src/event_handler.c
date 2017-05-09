#ifndef EVENT_HANDLER
#define EVENT_HANDLER

/*
 File:          event_handler.c
 Description:   Implementations for handling an event loop.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* Application includes */
#include "list.h"

#include "event_handler.h"

static List *events;
static struct timespec evt_time;
static struct timespec evt_ltime;
static float evt_tick = 0.0f;

void evt_tick()
{
    if (evt_ltime == NULL)
        clock_gettime(CLOCK_MONOTONIC, &evt_ltime);
    clock_gettime(CLOCK_MONOTONIC, &evt_time);

    float diff = ((float) evt_time.tv_sec - (float) evt_ltime.tv_sec) +
        ((float) evt_time.tv_nsec - (float) evt_ltime.tv_nsec) / 1000000000.0f;
    evt_ltime = evt_time;
    
    evt_tick += diff;

    if (event_checkdone(events, evt_tick))
    {
        struct event *evt_data;
        evt_data = (struct event *) list_pop(&events);
        printf("Event type is: %d\n", evt_data->type);
        evt_tick = 0.0f;       
    }
}

void event_add(int event_type, float duration)
{
    struct event *evt = malloc(sizeof(evt));
    
    evt->type = event_type;
    evt->duration = duration;

    list_push(&events, (void *) evt);
}

bool event_checkdone(List *event, float secs)
{
    struct event *data;
    data = (struct event *) event->data;

    if (secs > event->duration)
        return true;

    return false;
}

#endif