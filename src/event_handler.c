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
    float modifier = (float) QuarticEaseIn((AHFloat) complete);

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
    Event *evt = malloc(sizeof(Event));
    
    evt->type = event_type;
    evt->duration = duration;

    switch (event_type)
    {
        case EVENT_RESET:
            break;
        case EVENT_WALK_A:
            evt->mvmts = (Keyframe *) &keyfr_walka;
            break;
        case EVENT_WALK_B:
            evt->mvmts = (Keyframe *) &keyfr_walkb;
            break;            
        case EVENT_UP:
            evt->mvmts = (Keyframe *) &keyfr_up;
            break;
    }

    list_push(&events, (void *) evt);
}

bool event_checkdone(List *events, float secs)
{
    Event *event = (Event *) events->data;

    if (secs > event->duration)
        return true;

    return false;
}

#endif