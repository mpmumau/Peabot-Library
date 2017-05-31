#ifndef EVENTS_DEF
#define EVENTS_DEF
#define _GNU_SOURCE
/*
 File:          events.c
 Description:   Implementations for handling an event loop.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <sys/prctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

/* Application includes */
#include "config_defaults.h"
#include "main.h"
#include "config.h"
#include "log.h"
#include "list.h"
#include "event_callbacks.h"

/* Header */
#include "events.h"

static pthread_t event_thread;
static bool running = true;
static List *events;

/* Forward decs */
static void *event_main(void *arg);
static char *event_getname(unsigned short event_type);

void event_init()
{
    events = calloc(1024, sizeof(List));
    if (!events)
        APP_ERROR("Could not calloc", 1);

    int error = pthread_create(&event_thread, NULL, event_main, NULL);
    if (error)
        APP_ERROR("Could not create thread.", error);
}

void event_halt()
{
    running = false;
    int error = pthread_join(event_thread, NULL);
    if (error)
        log_event("[ERROR!] Could not rejoin from robot thread.");
    free(events);
    events = NULL;
}

static void *event_main(void *arg)
{
    prctl(PR_SET_NAME, "PEABOT_EVENTS\0", NULL, NULL, NULL);

    Event *event;
    void (*event_callback)(void *arg);

    while (running)
    {
        event = (Event *) list_pop(&events);
        event_callback = NULL;

        if (!event)
            continue;

        if (event->type == EVENT_RESET)
            event_callback = eventcb_reset;

        if (event->type == EVENT_DELAY)
            event_callback = eventcb_delay;

        if (event->type == EVENT_ELEVATE)
            event_callback = eventcb_elevate;

        if (event->type == EVENT_EXTEND)
            event_callback = eventcb_extend;        

        if (event->type == EVENT_WALK)
            event_callback = eventcb_walk;

        if (event->type == EVENT_TURN)
            event_callback = eventcb_turn;

        if (event->type == EVENT_HALT)
            event_callback = eventcb_halt;

        if (!event_callback)
            continue;

        (*event_callback)(event->data);

        if (event->data)
            free(event->data);
        free(event);
    }

    free(events);
    return (void *) NULL;
}

void event_add(unsigned short event_type, void *data)
{
    static List **events_pp = &events;

    Event *event = calloc(1, sizeof(Event));
    event->type = event_type;
    event->data = data;

    bool *log_event_add = config_get(CONF_LOG_EVENT_ADD);
    if (*log_event_add)
    {
        char log_msg[LOG_LINE_MAXLEN];
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Added event. (type: %s)", event_getname(event_type));
        log_event(log_msg);
    }

    list_push(events_pp, (void *) event);
}

static char *event_getname(int event_type)
{
    switch (event_type)
    {
        case EVENT_RESET:
            return "EVENT_RESET";
        case EVENT_DELAY:
            return "EVENT_DELAY";
        case EVENT_ELEVATE:
            return "EVENT_ELEVATE";
        case EVENT_WALK:
            return "EVENT_WALK";
        case EVENT_EXTEND:
            return "EVENT_EXTEND";
        case EVENT_TURN:
            return "EVENT_TURN";
        case EVENT_HALT:
            return "EVENT_HALT";
    }

    return NULL;
}

#endif