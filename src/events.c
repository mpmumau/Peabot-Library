#ifndef EVENTS_DEF
#define EVENTS_DEF

/*
 File:          events.c
 Description:   Implementations for handling an event loop.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

#define _GNU_SOURCE

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
static void event_destroy(Event *event);
static void *event_main(void *arg);
static char *event_getname(unsigned short event_type);
static void event_log_eventadd(Event *event);
static void *event_get_data_cpy(unsigned short event_type, void *data);

void event_init()
{
    int error = pthread_create(&event_thread, NULL, event_main, NULL);
    if (error)
        APP_ERROR("Could not create thread.", error);
}

void event_halt()
{
    running = false;
    int error = pthread_join(event_thread, NULL);
    if (error)
        log_error("Could not rejoin from robot thread.", error);

    if (events != NULL)
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
        if (!event)
            continue;

        event_callback = NULL;

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

        event_destroy(event);
        event = NULL;
    }

    if (events)
        free(events);
    events = NULL;

    return (void *) NULL;
}

void event_add(unsigned short event_type, void *data)
{
    Event *event = calloc(1, sizeof(Event));
    if (!event)
        APP_ERROR("Could not allocate memory.", 1);
  
    void *data_cpy = event_get_data_cpy(event_type, data);

    event->type = event_type;
    event->data = data_cpy;

    list_push(&events, (void *) event);
    event_log_eventadd(event);
}

static void event_destroy(Event *event)
{
    if (!event)
        return;

    if (event->data)
        free(event->data);
    event->data = NULL;

    if (event)
        free(event);
    event = NULL;    
}

static void *event_get_data_cpy(unsigned short event_type, void *data)
{
    if (!data)
        return NULL;

    void *data_p = NULL;

    switch (event_type)
    {
        case EVENT_DELAY:
            data_p = (void *) calloc(1, sizeof(double));            
            break;
        case EVENT_ELEVATE:
            data_p = (void *) calloc(1, sizeof(EventElevateData));
            break;
        case EVENT_WALK:
            data_p = (void *) calloc(1, sizeof(EventWalkData));
            break;
        case EVENT_EXTEND:
            data_p = (void *) calloc(1, sizeof(EventExtendData));
            break;
        case EVENT_TURN:
            data_p = (void *) calloc(1, sizeof(EventTurnData));   
            break;     
        default:
            return NULL;                            
    }

    if (!data_p)
        APP_ERROR("Could not allocate memory.", 1);      

    switch (event_type)
    {
        case EVENT_DELAY:
            double *delay_p = (double *) data_p;   
            double *delay_val_p = (double *) data;     
            *delay_p = *delay_val_p;   
            break;
        case EVENT_ELEVATE:
            EventElevateData *elevate_p = (EventElevateData *) data_p;
            EventElevateData *elevate_val_p = (EventElevateData *) data;
            *elevate_p = *elevate_val_p;
            break;
        case EVENT_WALK:
            EventWalkData *walk_p = (EventWalkData *) data_p;
            EventWalkData *walk_val_p = (EventWalkData *) data;
            *walk_p = *walk_val_p;
            break;
        case EVENT_EXTEND:
            EventExtendData *extend_p = (EventExtendData *) data_p;
            EventExtendData *extend_val_p = (EventExtendData *) data;
            break;
        case EVENT_TURN:
            EventTurnData *turn_p = (EventTurnData *) data_p;
            EventTurnData *turn_val_p = (EventTurnData *) data;
            *turn_p = *turn_val_p;
            break;                                
    }    

    return data_p;
}

static char *event_getname(unsigned short event_type)
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

static void event_log_eventadd(Event *event)
{
    bool *log_event_add = config_get(CONF_LOG_EVENT_ADD);
    if (!*log_event_add)
        return;

    char log_msg[LOG_LINE_MAXLEN];
    snprintf(log_msg, sizeof(log_msg), "[Event] Added event. (type: %s)", event_getname(event->type));
    log_event(log_msg);  
}

#endif