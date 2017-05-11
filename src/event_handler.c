#ifndef EVENT_HANDLER_DEF
#define EVENT_HANDLER_DEF

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
#include "keyframe.h"
#include "robot.h"
#include "easing.h"
#include "event_add_callbacks.h"

#include "event_handler.h"

static List *events;

/* Forward decs */
bool event_checkdone(List *event, float secs);

void event_tick()
{
    static struct timespec time;
    static struct timespec last_time;
    static float next = 0.0f;
    static Event *evt_data;

    clock_gettime(CLOCK_MONOTONIC, &time);
    next += utils_timediff(time, last_time);
    last_time = time;

    if (!events)
    {
        next = 0.0f;
        return;
    }

    if (event_checkdone(events, next))
    {
        Event *tmp_data = list_pop(&events);
        free(tmp_data);
        
        next = 0.0f; 
        return;      
    }

    if (!events->data)
        return;

    evt_data = (Event *) events->data;

    int evt_type = evt_data->type;
    Keyframe *mvmts = evt_data->mvmts;

    if (!mvmts || evt_type == EVENT_DELAY)
        return;

    float complete = next / evt_data->duration;
    float modifier = (float) CubicEaseInOut((AHFloat) complete);

    for (int i = 0; i < SERVOS_NUM; i++)
    {
        float diff = mvmts->end_pos - mvmts->start_pos;
        float diff_mod = diff * modifier;
        float final = mvmts->start_pos + diff_mod;

        robot_setservo(i, final);

        mvmts++;
    }
}

void event_add(int event_type, float duration)
{
    void (*event_add_cb)(List *events, float duration) = NULL;

    if (event_type == EVENT_WALK_A)
        event_add_cb = eventadd_walka;

    if (event_type == EVENT_WALK_B)
        event_add_cb = eventadd_walkb;

    if (event_type == EVENT_UP)
        event_add_cb = eventadd_up;

    if (event_add_cb == NULL)
        return;
    (*event_add_cb)(events, duration);
}

bool event_checkdone(List *events, float secs)
{
    Event *event = (Event *) events->data;

    if (secs > event->duration)
        return true;

    return false;
}

#endif