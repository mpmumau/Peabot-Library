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
#include "config_defaults.h"
#include "main.h"
#include "config.h"
#include "log.h"
#include "events.h"
#include "keyframe_handler.h"

/* Header */
#include "event_callbacks.h"

void eventcb_reset(void *arg)
{
    bool *log_event_callbacks = config_get(CONF_LOG_EVENT_CALLBACKS);
    if (*log_event_callbacks)
    {
        char *log_msg = calloc(sizeof(char) * LOG_LINE_MAXLEN);
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Adding KEYFR_HOME keyframe.");
        log_event(log_msg);
        free(log_msg);
    }

    keyhandler_add(KEYFR_HOME, (void *) NULL, false, true);
}

void eventcb_delay(void *arg)
{
    float *dp = (float *) arg;
    float duration_val = *dp;

    float *duration = calloc(sizeof(float));
    if (!duration)
        app_exit("[ERROR!] Failed to allocate memory for float (eventcb_delay).", 1);
    *duration = duration_val;

    bool *log_event_callbacks = config_get(CONF_LOG_EVENT_CALLBACKS);
    if (*log_event_callbacks)
    {
        char *log_msg = malloc(sizeof(char) * LOG_LINE_MAXLEN);
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Adding KEYFR_DELAY keyframe. (duration: %f)", *duration);
        log_event(log_msg);
        free(log_msg);
    }

    keyhandler_add(KEYFR_DELAY, (void *) duration, false, true);
}

void eventcb_elevate(void *arg)
{
    EventElevateData *elevate_data = (EventElevateData *) arg;
    bool reverse = elevate_data->reverse;

    float *duration = malloc(sizeof(float));
    if (!duration)
        app_exit("[ERROR!] Failed to allocate memory for float (eventcb_elevate).", 1);
    *duration = elevate_data->duration;

    bool *log_event_callbacks = config_get(CONF_LOG_EVENT_CALLBACKS);
    if (*log_event_callbacks)
    {
        char *log_msg = malloc(sizeof(char) * LOG_LINE_MAXLEN);
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Adding KEYFR_ELEVATE keyframe. (duration: %f, reverse: %d)", *duration, (int) reverse);
        log_event(log_msg);
        free(log_msg);
    }    

    keyhandler_add(KEYFR_ELEVATE, (void *) duration, reverse, false);
}

void eventcb_extend(void *arg)
{
    EventExtendData *extend_data = (EventExtendData *) arg;
    bool reverse = extend_data->reverse;

    float *duration = malloc(sizeof(float));
    if (!duration)
        app_exit("[ERROR!] Failed to allocate memory for float (eventcb_elevate).", 1);
    *duration = extend_data->duration;

    bool *log_event_callbacks = config_get(CONF_LOG_EVENT_CALLBACKS);
    if (*log_event_callbacks)
    {
        char *log_msg = malloc(sizeof(char) * LOG_LINE_MAXLEN);
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Adding KEYFR_EXTEND keyframe. (duration: %f, reverse: %d)", *duration, (int) reverse);
        log_event(log_msg);
        free(log_msg);
    }    

    keyhandler_add(KEYFR_EXTEND, (void *) duration, reverse, false);
}

void eventcb_walk(void *arg)
{
    EventWalkData *walk_data = (EventWalkData *) arg;

    int cycles = walk_data->cycles;
    float duration = walk_data->duration;

    bool *log_event_callbacks = config_get(CONF_LOG_EVENT_CALLBACKS);
    if (*log_event_callbacks)
    {
        char *log_msg = malloc(sizeof(char) * LOG_LINE_MAXLEN);
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Event] Adding KEYFR_WALK keyframes. (duration: %f, cycles: %d)", duration, cycles);
        log_event(log_msg);
        free(log_msg);
    } 

    float *duration_p;

    duration_p = malloc(sizeof(float));
    if (!duration)
        app_exit("[ERROR!] Failed to allocate memory for float (eventcb_walk).", 1);     
    *duration_p = duration;
           
    keyhandler_add(KEYFR_WALK, (void *) duration_p, true, false);    

    for (int i = 0; i < cycles - 1; i++)
    {
        duration_p = malloc(sizeof(float));
        if (!duration_p)
            app_exit("[ERROR!] Failed to allocate memory for float (eventcb_walk).", 1);
        *duration_p = duration;
        
        keyhandler_add(KEYFR_WALK, (void *) duration_p, false, true);

        duration_p = malloc(sizeof(float));
        if (!duration)
            app_exit("[ERROR!] Failed to allocate memory for float (eventcb_walk).", 1);     
        *duration_p = duration;
               
        keyhandler_add(KEYFR_WALK, (void *) duration_p, true, true);
    }

    duration_p = malloc(sizeof(float));
    if (!duration_p)
        app_exit("[ERROR!] Failed to allocate memory for float (eventcb_walk).", 1);
    *duration_p = duration;
    
    keyhandler_add(KEYFR_WALK, (void *) duration_p, false, true);    
}

#endif