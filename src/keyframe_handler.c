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
#include "main.h"
#include "config_defaults.h"
#include "config.h"
#include "log.h"
#include "list.h"
#include "easing_utils.h"
#include "utils.h"
#include "robot.h"
#include "keyframe_factory.h"

/* Header */
#include "keyframe_handler.h"

static pthread_t keyhandler_thread;
static bool running;

static List *keyframes = NULL;
static Keyframe *last_keyfr = NULL;  

/* Forward decs */
static void *keyhandler_main(void *arg);
static float keyhandler_mappos(float perc, ServoPos *servo_pos);

void keyhandler_init()
{
    running = true;
    int error = pthread_create(&keyhandler_thread, NULL, keyhandler_main, NULL);
}

void keyhandler_halt()
{
    running = false;

    free(last_keyfr->servo_pos);
    free(last_keyfr);

    pthread_join(keyhandler_thread, NULL);
}

void keyhandler_add(int keyfr_type, void *data, bool reverse, bool skip_transitions)
{
    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);
    bool *transitions_enable = (bool *) config_get(CONF_TRANSITIONS_ENABLE);
    float *transitions_time = (float *) config_get(CONF_TRANSITIONS_TIME);

    if (last_keyfr == NULL)
        last_keyfr = calloc(1, sizeof(Keyframe));

    if (last_keyfr->servo_pos == NULL)
        last_keyfr->servo_pos = calloc(*servos_num, sizeof(ServoPos));

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

    if (keyfr_type == KEYFR_EXTEND)
        keyfactory_cb = keyfactory_extend;

    if (keyfr_type == KEYFR_TURN)
        keyfactory_cb = keyfactory_turnsegment;

    if (keyfactory_cb == NULL)
        return;

    keyfr = (*keyfactory_cb)(data, reverse);

    if (!keyfr)
        return;

    if (keyfr_type != KEYFR_DELAY &&
        *transitions_enable && 
        !skip_transitions)
    {
        KeyframeTransData trans_data;
        trans_data.duration = *transitions_time;
        trans_data.src = last_keyfr->servo_pos;
        trans_data.dest = keyfr->servo_pos;
        
        Keyframe *trans_keyfr = keyfactory_transition(trans_data);

        if (trans_keyfr != NULL)
        {
            list_push(&keyframes, (void *) trans_keyfr);
            keyhandler_add(keyfr_type, data, reverse, true);

            last_keyfr->is_delay = trans_keyfr->is_delay;
            last_keyfr->duration = trans_keyfr->duration;

            for (int q = 0; q < *servos_num; q++)
            {
                last_keyfr->servo_pos[q].easing = trans_keyfr->servo_pos[q].easing;
                last_keyfr->servo_pos[q].start_pos = trans_keyfr->servo_pos[q].start_pos;
                last_keyfr->servo_pos[q].end_pos = trans_keyfr->servo_pos[q].end_pos;
                last_keyfr->servo_pos[q].begin_pad = trans_keyfr->servo_pos[q].begin_pad;
                last_keyfr->servo_pos[q].end_pad = trans_keyfr->servo_pos[q].end_pad;      
            }

            return;
        }
    }

    if (data)
        free(data);    

    list_push(&keyframes, (void *) keyfr);

    last_keyfr->is_delay = keyfr->is_delay;
    last_keyfr->duration = keyfr->duration;

    if (keyfr->servo_pos == NULL)
        return;

    for (int r = 0; r < *servos_num; r++)
    {
        last_keyfr->servo_pos[r].easing = keyfr->servo_pos[r].easing;
        last_keyfr->servo_pos[r].start_pos = keyfr->servo_pos[r].start_pos;
        last_keyfr->servo_pos[r].end_pos = keyfr->servo_pos[r].end_pos;
        last_keyfr->servo_pos[r].begin_pad = keyfr->servo_pos[r].begin_pad;
        last_keyfr->servo_pos[r].end_pad = keyfr->servo_pos[r].end_pad;        
    }
}

static void *keyhandler_main(void *arg)
{
    struct timespec time;
    struct timespec last_time;
    double next = 0.0;
    
    Keyframe *keyfr;
    ServoPos *servo_pos;
    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);
    
    float perc, pos, begin_time, end_time, adjusted_duration;

    char msg[LOG_LINE_MAXLEN];

    while (running)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        next += utils_timediff(time, last_time);
        last_time = time;        

        if (!keyframes)
        {
            next = 0.0;
            continue;       
        }

        keyfr = (Keyframe *) keyframes->data;
        
        if (keyfr->servo_pos != NULL)
            servo_pos = keyfr->servo_pos;
        else 
            servo_pos = NULL;

        if (!keyfr->is_delay && servo_pos)
        {
            for (int i = 0; i < *servos_num; i++)
            {
                begin_time = keyfr->duration * servo_pos[i].begin_pad;
                end_time = keyfr->duration * servo_pos[i].end_pad;
                adjusted_duration = keyfr->duration - begin_time - end_time;  
                
                perc = (next - begin_time) / adjusted_duration;

                if (perc < 0.0)
                    perc = 0.0;
                if (perc > 1.0)
                    perc = 1.0;

                pos = keyhandler_mappos(perc, &servo_pos[i]);
                
                robot_setservo(i, pos);
            }
        }

        if (next > keyfr->duration)
        {
            bool *log_keyframes = (bool *) config_get(CONF_LOG_KEYFRAMES);
            if (*log_keyframes)
            {
                snprintf(msg, LOG_LINE_MAXLEN, "[Keyfr] Completed keyframe. (duration: %f, is_delay: %d)", keyfr->duration, (int) keyfr->is_delay);
                log_event(msg);
            }     

            next = 0.0;

            if (keyfr->is_delay) 
            {
                list_pop(&keyframes);
                continue;
            }

            if (servo_pos != NULL)
            {
                free(servo_pos);
                servo_pos = NULL;
            }     

            Keyframe *keyfr_popped = NULL;
            keyfr_popped = (Keyframe *) list_pop(&keyframes);
            if (keyfr_popped)
                free(keyfr_popped);
        }
    }

    return (void *) NULL;
}

static float keyhandler_mappos(float perc, ServoPos *servo_pos)
{
    float diff, modifier, delta, final;

    diff = servo_pos->end_pos - servo_pos->start_pos;
    modifier = easing_calc(servo_pos->easing, perc);
    delta = diff * modifier;    
    final = servo_pos->start_pos + delta;

    return final;
}

#endif