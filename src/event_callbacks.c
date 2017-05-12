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
    keyhandler_add(KEYFR_RESET, NULL, false);
}

void eventcb_delay(void *arg)
{
    float *duration_p = (float *) arg;
    keyhandler_add(KEYFR_DELAY, *duration_p, false);
}

void eventcb_elevate(void *arg)
{
    EventElevateData *elevate_data = (EventElevateData *) arg;
    bool reverse = elevate_data->reverse;
    float duration = elevate_data->duration;

    keyhandler_add(KEYFR_ELEVATE, *duration_p, false);
}

void eventcb_walk(void *arg)
{
    EventWalkData *walk_data = (EventWalkData *) arg;

    int cycles = walk_data->cycles;
    float *duration = malloc(sizeof(float));
    *duration = walk_data->duration;

    for (int i = 0; i < cycles; i++)
    {
        keyhandler_add(KEYFR_WALK, (void *) duration, false);
        keyhandler_add(KEYFR_WALK, (void *) duration, true);
    }
}

#endif