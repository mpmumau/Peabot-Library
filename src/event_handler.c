#ifndef EVENT_HANDLER
#define EVENT_HANDLER

/*
 File:          event_handler.c
 Description:   Implementations for handling an event loop.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

#define _POSIX_C_SOURCE 199309L

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* Application includes */
#include "list.h"
#include "utils.h"

#include "event_handler.h"

static List *events;

/* Forward decs */
bool event_checkdone(List *event, float secs);

void event_tick()
{
    if (events == NULL)
        return;

    static struct timespec time;
    static struct timespec last_time;
    static float next = 0.0f;
    static struct event *evt_data;

    clock_gettime(CLOCK_MONOTONIC, &time);

    next += utils_timediff(time, last_time);
    printf("Next is: &f\n", next);

    if (event_checkdone(events, next))
    {
        struct event *tmp_data = list_pop(&events);
        free(tmp_data);
        evt_data = (struct event *) events->data;
        next = 0.0f;       
    }

    printf("Event type is: %d\n", evt_data->type);
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

    if (secs > data->duration)
        return true;

    return false;
}

#endif