#ifndef EVENT_CALLBACKS_DEF
#define EVENT_CALLBACKS_DEF

/*
 File:          event_callbacks.c
 Description:   Callback functions for events.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdio.h>
#include <stdlib.h>

/* Application includes */
#include "main.h"
#include "config.h"
#include "log.h"
#include "events.h"
#include "keyframe_handler.h"

/* Header */
#include "event_callbacks.h"

void eventcb_reset(void *arg)
{
    if (LOG_EVENT_CALLBACKS)
    {
        char *log_msg = malloc(sizeof(char) * LOG_LINE_MAXLEN);
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Adding KEYFR_HOME keyframe.");
        log_event(log_msg);
        free(log_msg);
    }

    keyhandler_add(KEYFR_HOME, (void *) NULL, false);
}

void eventcb_delay(void *arg)
{
    float *dp = (float *) arg;
    float duration = *dp;

    float *duration_p = malloc(sizeof(float));
    if (!duration_p)
        app_exit("[ERROR!] Failed to allocate memory for float (eventcb_delay).", 1);
    *duration_p = duration;

    if (LOG_EVENT_CALLBACKS)
    {
        char *log_msg = malloc(sizeof(char) * LOG_LINE_MAXLEN);
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Adding KEYFR_DELAY keyframe. (duration: %f)", *duration_p);
        log_event(log_msg);
        free(log_msg);
    }

    printf("Duration here: %f\n", *duration_p);

    keyhandler_add(KEYFR_DELAY, (void *) duration_p, false);
}

void eventcb_elevate(void *arg)
{
    EventElevateData *elevate_data = (EventElevateData *) arg;
    bool reverse = elevate_data->reverse;

    float *duration = malloc(sizeof(float));
    if (!duration)
        app_exit("[ERROR!] Failed to allocate memory for float (eventcb_elevate).", 1);
    *duration = elevate_data->duration;

    if (LOG_EVENT_CALLBACKS)
    {
        char *log_msg = malloc(sizeof(char) * LOG_LINE_MAXLEN);
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Adding KEYFR_ELEVATE keyframe. (duration: %f, reverse: %d)", *duration, (int) reverse);
        log_event(log_msg);
        free(log_msg);
    }    

    keyhandler_add(KEYFR_ELEVATE, (void *) duration, reverse);
}

void eventcb_walk(void *arg)
{
    EventWalkData *walk_data = (EventWalkData *) arg;

    int cycles = walk_data->cycles;
    float duration = walk_data->duration;

    if (LOG_EVENT_CALLBACKS)
    {
        char *log_msg = malloc(sizeof(char) * LOG_LINE_MAXLEN);
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Adding KEYFR_WALK keyframes. (duration: %f, cycles: %d)", duration, cycles);
        log_event(log_msg);
        free(log_msg);
    } 

    float *duration_p;

    for (int i = 0; i < cycles; i++)
    {
        duration_p = malloc(sizeof(float));
        if (!duration_p)
            app_exit("[ERROR!] Failed to allocate memory for float (eventcb_walk).", 1);
        *duration_p = duration;
        
        keyhandler_add(KEYFR_WALK, (void *) duration_p, false);

        duration_p = malloc(sizeof(float));
        if (!duration)
            app_exit("[ERROR!] Failed to allocate memory for float (eventcb_walk).", 1);     
        *duration_p = duration;
               
        keyhandler_add(KEYFR_WALK, (void *) duration_p, true);
    }
}

#endif