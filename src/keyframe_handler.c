#ifndef KEYFRAME_HANDLER_DEF
#define KEYFRAME_HANDLER_DEF

/*
 File:          keyframe_handler.c
 Description:   Implementation of keyframe animation processing functions.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

#define _POSIX_C_SOURCE 199309L

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

/* Application includes */
#include "config.h"
#include "list.h"
#include "easing.h"
#include "utils.h"
#include "robot.h"
#include "keyframe_factory.h"

/* Header */
#include "keyframe_handler.h"

static pthread_t keyhandler_thread;
static bool running = true;

static List *keyframes;
static Keyframe *last_keyfr;  

/* Forward decs */
static void *keyhandler_main(void *arg);
static float keyhandler_mappos(float perc, ServoPos *servo_pos);

void keyhandler_init()
{
    pthread_create(&keyhandler_thread, NULL, keyhandler_main, NULL);
}

void keyhandler_halt()
{
    running = false;
    pthread_join(keyhandler_thread, NULL);
}

void keyhandler_add(int keyfr_type, void *data, bool reverse)
{
    Keyframe *keyfr;
    Keyframe *(*keyfactory_cb)(void *data, bool reverse);

    keyfactory_cb = NULL;

    if (keyfr_type == KEYFR_HOME)
        keyfactory_cb = keyfactory_home;

    if (keyfr_type == KEYFR_DELAY)
        keyfactory_cb = keyfactory_delay;

    if (keyfr_type == KEYFR_ELEVATE)
        keyfactory_cb = keyfactory_elevate;

    if (keyfr_type == KEYFR_WALK)
        keyfactory_cb = keyfactory_walk;

    if (keyfactory_cb == NULL)
        return;

    keyfr = (*keyfactory_cb)(data, reverse);

    if (!keyfr)
        return;

    if (keyfr_type != KEYFR_DELAY)
    {
        // Check if the new animation needs a transition keyframe and insert it first if so.
        Keyframe *tmp_keyfr;
        if (last_keyfr)
            tmp_keyfr = last_keyfr;
        else
            tmp_keyfr = keyfactory_home((void *) NULL, false);

        KeyframeTransData *trans_data = malloc(sizeof(KeyframeTransData));
        trans_data->duration = KEYFRAME_TRANSITION_TIME;
        trans_data->src = tmp_keyfr->servo_pos;
        trans_data->dest = keyfr->servo_pos;
        
        Keyframe *trans_keyfr = keyfactory_transition((void *) trans_data, false);

        if (trans_keyfr)
            list_push(&keyframes, trans_keyfr);
    }

    list_push(&keyframes, keyfr);
}

static void *keyhandler_main(void *arg)
{
    struct timespec time;
    struct timespec last_time;
    float next = 0.0f;
    Keyframe *keyfr;
    ServoPos *servo_pos;
    float perc;
    float pos;

    while (running)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        next += utils_timediff(time, last_time);
        last_time = time;        

        if (!keyframes)
        {
            next = 0.0f;
            continue;       
        }

        keyfr = (Keyframe *) keyframes->data;
        servo_pos = keyfr->servo_pos;
        
        perc = next / keyfr->duration;
        if (perc > 1.0f)
            perc = 1.0f;
        if (perc < 0.0f)
            perc = 0.0f;

        for (int i = 0; i < SERVOS_NUM; i++)
        {
            pos = keyhandler_mappos(perc, &servo_pos[i]);
            robot_setservo(i, pos);
        }

        if (next > keyfr->duration)
        {
            next = 0.0f;

            if (last_keyfr && last_keyfr->servo_pos)
            {
                free(last_keyfr->servo_pos);
                free(last_keyfr);                
            }

            last_keyfr = (Keyframe *) list_pop(&keyframes);
        }
    }

    return (void *) NULL;
}

static float keyhandler_mappos(float perc, ServoPos *servo_pos)
{
    float diff, mod, delta, final;

    diff = servo_pos->end_pos - servo_pos->start_pos;
    mod = easing_calc(servo_pos->easing, perc);
    delta = diff * mod;
    final = servo_pos->start_pos + delta;

    return final;
}

#endif