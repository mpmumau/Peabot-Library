#ifndef EVENT_CALLBACKS_DEF
#define EVENT_CALLBACKS_DEF

/*
 File:          event_callbacks.c
 Description:   Callback functions for events.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

/* Application includes */
#include "events.h"
#include "keyframe_handler.h"

/* Header */
#include "event_callbacks.h"

void eventcb_reset(void *arg)
{
    keyhandler_add(KEYFR_HOME, (void *) NULL, false);
}

void eventcb_delay(void *arg)
{
    float *duration = malloc(sizeof(float));
    if (!duration)
        app_exit("[ERROR!] Failed to allocate memory for float (eventcb_delay).", 1);

    *duration = (float) *arg;

    keyhandler_add(KEYFR_DELAY, (void *) duration, false);
}

void eventcb_elevate(void *arg)
{
    EventElevateData *elevate_data = (EventElevateData *) arg;
    bool reverse = elevate_data->reverse;

    float *duration = malloc(sizeof(float));
    if (!duration)
        app_exit("[ERROR!] Failed to allocate memory for float (eventcb_elevate).", 1);

    *duration = elevate_data->duration;

    keyhandler_add(KEYFR_ELEVATE, (void *) duration, reverse);
}

void eventcb_walk(void *arg)
{
    EventWalkData *walk_data = (EventWalkData *) arg;

    int cycles = walk_data->cycles;
    float duration = walk_data->duration;

    float *duration_p;

    for (int i = 0; i < cycles; i++)
    {
        duration_p = malloc(sizeof(float));
        if (!duration)
            app_exit("[ERROR!] Failed to allocate memory for float (eventcb_walk).", 1);
        keyhandler_add(KEYFR_WALK, (void *) duration_p, false);

        duration_p = malloc(sizeof(float));
        if (!duration)
            app_exit("[ERROR!] Failed to allocate memory for float (eventcb_walk).", 1);        
        keyhandler_add(KEYFR_WALK, (void *) duration_p, true);
    }
}

#endif