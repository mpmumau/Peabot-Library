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

/* Forward decs */
static void eventcb_logcb(const char *msg);

void eventcb_reset(void *arg)
{
    keyhandler_add(KEYFR_RESET, (void *) NULL, false, false);
    eventcb_logcb("Added KEYFR_RESET keyframe.");
}

void eventcb_delay(void *arg)
{
    double *dp = (double *) arg;
    double duration_val = *dp;

    double *duration = calloc(1, sizeof(double));
    if (!duration)
        APP_ERROR("Could not allocate memory.", 1);

    *duration = duration_val;

    keyhandler_add(KEYFR_DELAY, (void *) duration, false, true);
    eventcb_logcb("Added KEYFR_DELAY keyframe.");
}

void eventcb_elevate(void *arg)
{
    EventElevateData *elevate_data = (EventElevateData *) arg;
    bool reverse = elevate_data->reverse;

    double *duration = calloc(1, sizeof(double));
    if (!duration)
        APP_ERROR("Could not allocate memory.", 1);

    *duration = elevate_data->duration; 

    keyhandler_add(KEYFR_ELEVATE, (void *) duration, reverse, false);
    eventcb_logcb("Added KEYFR_ELEVATE keyframe.");
}

void eventcb_extend(void *arg)
{
    EventExtendData *extend_data = (EventExtendData *) arg;
    bool reverse = extend_data->reverse;

    double *duration = calloc(1, sizeof(double));
    if (!duration)
        APP_ERROR("Could not allocate memory.", 1);

    *duration = extend_data->duration;

    keyhandler_add(KEYFR_EXTEND, (void *) duration, reverse, false);
    eventcb_logcb("Added KEYFR_EXTEND keyframe.");
}

void eventcb_walk(void *arg)
{
    EventWalkData *walk_data = (EventWalkData *) arg;

    unsigned short cycles = walk_data->cycles;
    double duration = walk_data->duration;
    bool reverse = walk_data->reverse;

    double *duration_p;   

    for (unsigned short i = 0; i < cycles; i++)
    {
        duration_p = calloc(1, sizeof(double));
        if (!duration_p)
            APP_ERROR("Could not allocate memory.", 1);

        *duration_p = duration;
        
        keyhandler_add(KEYFR_WALK, (void *) duration_p, reverse, i > 0);
    }

    keyhandler_add(KEYFR_ELEVATE, (void *) NULL, false, false);

    eventcb_logcb("Added KEYFR_WALK keyframes.");
}

void eventcb_turn(void *arg)
{
    EventTurnData *turn_data = (EventTurnData *) arg;

    unsigned short cycles = turn_data->cycles;
    double duration = turn_data->duration;
    bool reverse = turn_data->reverse;
   
    double *duration_p;

    for (unsigned short i = 0; i < cycles; i++)
    {
        duration_p = calloc(1, sizeof(double));
        if (!duration_p)
            APP_ERROR("Could not allocate memory.", 1);

        *duration_p = duration;
        
        keyhandler_add(KEYFR_TURN, (void *) duration_p, reverse, i > 0);
    }

    eventcb_logcb("Added KEYFR_TURN keyframes.");
}

void eventcb_strafe(void *arg)
{
    EventStrafeData *strafe_data = (EventStrafeData *) arg;

    unsigned short cycles = strafe_data->cycles;
    double duration = strafe_data->duration;
    bool reverse = strafe_data->reverse;
   
    double *duration_p;

    for (unsigned short i = 0; i < cycles; i++)
    {
        duration_p = calloc(1, sizeof(double));
        if (!duration_p)
            APP_ERROR("Could not allocate memory.", 1);

        *duration_p = duration;
        
        keyhandler_add(KEYFR_STRAFE, (void *) duration_p, reverse, i > 0);
    }

    keyhandler_add(KEYFR_RESET, (void *) NULL, false, false);
    eventcb_logcb("Added KEYFR_STRAFE keyframes.");
}

void eventcb_halt(void *arg)
{
    keyhandler_removeall();
    keyhandler_add(KEYFR_RESET, (void *) NULL, false, false);
    eventcb_logcb("Cleared all keyframes.");
}

static void eventcb_logcb(const char *msg)
{
    bool *log_event_callbacks = config_get(CONF_LOG_EVENT_CALLBACKS);
    if (!*log_event_callbacks)
        return;

    char log_msg[LOG_LINE_MAXLEN];
    snprintf(log_msg, sizeof(log_msg), "[EVNT] %s", msg);
    log_event(log_msg);
}

#endif