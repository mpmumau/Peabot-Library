#ifndef EVENT_HANDLER
#define EVENT_HANDLER

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
#include "movements.h"
#include "robot.h"

#include "event_handler.h"

static List *events;

/* Forward decs */
bool event_checkdone(List *event, float secs);

void event_tick()
{
    static struct timespec time;
    static struct timespec last_time;
    static float next = 0.0f;
    static struct event *evt_data;

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
        struct event *tmp_data = list_pop(&events);
        printf("Event ended; type was: %d\n", tmp_data->type);
        free(tmp_data);
        if (events && events->data)
            evt_data = (struct event *) events->data;
        next = 0.0f;       
    }
    else
    {
        if (!events->data)
            return;

        evt_data = events->data;

        float complete = next / evt_data->duration;
        printf("Perc complete: %f\n", complete);
        // struct servo_mvmt *mvmts = evt_data->mvmts;
        // for (int i = 0; i < SERVOS_NUM; i++)
        // {
        //     float diff = mvmts->end_pos - mvmts->start_pos;
        //     float diff_mod = diff * perc_complete;
        //     float final = mvmts->start_pos + diff_mod;
        //     robot_setservo(i, final);
        //     mvmts++;
        // }
    }
}

void event_add(int event_type, float duration)
{
    struct event *evt = malloc(sizeof(evt));
    
    evt->type = event_type;
    evt->duration = duration;

    switch (event_type)
    {
        case EVENT_RESET:
            break;
        case EVENT_WALK_A:
            evt->mvmts = mvmt_walk_a();
            break;
    }

    list_push(&events, (void *) evt);
}

bool event_checkdone(List *event, float secs)
{
    struct event *data;
    data = (struct event *) event->data;

    if (secs > data->duration)
        return true;

    return false;
}

#endif