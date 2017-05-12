#ifndef EVENTS_DEF
#define EVENTS_DEF

/*
 File:          events.c
 Description:   Implementations for handling an event loop.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdbool.h>
#include <pthread.h>

/* Application includes */
#include "list.h"

/* Header */
#include "events.h"

static pthread_t event_thread;
static bool running;
static List *events;

/* Forward decs */
static void *event_main(void *arg);

void event_init()
{
    pthread_create(&event_thread, NULL, event_main, NULL);
}

void event_halt()
{
    running = false;
    pthread_join(event_thread, NULL);
}

static void *event_main(void *arg)
{
    Event *event;
    void (*event_callback)(void *arg);

    while (running)
    {
        if (!events)
            continue;
        
        event = (Event) list_pop(&events);

        if (event->type == EVENT_RESET)
            event_callback = eventcb_reset;

        if (event->type == EVENT_DELAY)
            event_callback = eventcb_delay;

        if (event->type == EVENT_ELEVATE)
            event_callback = eventcb_elevate;

        if (event->type == EVENT_WALK)
            event_callback = eventcb_walk;

        if ((*event_callback) != NULL)
            (*event_callback)(event->data);

        free(event->data);
        free(event);
    }

    return (void *) NULL;
}

void event_add(int event_type, void *data)
{
    Event *event = malloc(sizeof(Event));

    event->type = event_type;
    event->data = data;

    list_add(&events, (void *) event);
}

#endif