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

/* Application includes */
#include "list.h"

#include "event_handler.h"

List *events;

void event_tick()
{
    List *event = events;
    struct event *evt_data;
    while (event)
    {
        evt_data = (struct event *) event->data;
        printf("Event type is: %d\n", evt_data->type);
        event = event->next;
    }
}

void event_add(int event_type)
{
    struct event *evt = malloc(sizeof(evt));
    evt->type = event_type;

    list_push(&events, (void *) evt);
}

#endif