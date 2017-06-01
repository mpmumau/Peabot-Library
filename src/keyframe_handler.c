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
#include <sys/prctl.h>
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
static bool exec_remove_all = false;
static int error;

static List *keyframes = NULL;

/* Forward decs */
static void *keyhandler_main(void *arg);
static double keyhandler_mappos(double perc, ServoPos *servo_pos);
static void keyhandler_exec_removeall();
static void keyhandler_add_transition(size_t len, Keyframe *src, Keyframe *dest);
static void keyhandler_copy_keyfr(Keyframe *dest, Keyframe *src, size_t len);
static void keyhandler_log_keyfr(Keyframe *keyfr);

void keyhandler_init()
{
    running = true;
    error = pthread_create(&keyhandler_thread, NULL, keyhandler_main, NULL);
    if (error)
        APP_ERROR("Could not initialize keyframe thread.", error);
}

void keyhandler_halt()
{
    running = false;
    pthread_join(keyhandler_thread, NULL);
}

void keyhandler_add(unsigned short keyfr_type, void *data, bool reverse, bool skip_transitions)
{
    static Keyframe last_keyfr;
    static ServoPos last_servopos;
    if (last_keyfr.servo_pos == NULL)  
        last_keyfr.servo_pos = &last_servopos;

    unsigned short *servos_num = (unsigned short *) config_get(CONF_SERVOS_NUM);
    bool *transitions_enable = (bool *) config_get(CONF_TRANSITIONS_ENABLE);
    
    Keyframe *keyfr = calloc(1, sizeof(Keyframe));
    if (!keyfr)
        APP_ERROR("Could not allocate memory.", 1);

    ServoPos *servo_pos = calloc(*servos_num, sizeof(ServoPos));
    if (!servo_pos)
        APP_ERROR("Could not allocate memory.", 1);    
    keyfr->servo_pos = servo_pos;

    bool (*keyfactory_cb)(Keyframe *keyfr, size_t len, void *data, bool reverse);
    keyfactory_cb = NULL;

    if (keyfr_type == KEYFR_RESET)
        keyfactory_cb = keyfactory_reset;

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

    bool success = false;
    if (keyfactory_cb != NULL)
        success = (*keyfactory_cb)(keyfr, *servos_num, data, reverse);

    if (data)
        free(data);
    data = NULL;

    if (!success)
    {
        if (keyfr != NULL)
            free(keyfr);
        keyfr = NULL;

        if (servo_pos != NULL)
            free(servo_pos);
        servo_pos = NULL;

        return;
    }

    // Remember, the transition should come before the keyframe...
    if (keyfr_type != KEYFR_DELAY && *transitions_enable && !skip_transitions)
        keyhandler_add_transition(*servos_num, keyfr, &last_keyfr);

    list_push(&keyframes, (void *) keyfr);
    
    keyhandler_copy_keyfr(&last_keyfr, keyfr, *servos_num);
}

void keyhandler_removeall()
{
    exec_remove_all = true;
}

static void keyhandler_add_transition(size_t len, Keyframe *src, Keyframe *dest)
{
    Keyframe *keyfr = calloc(1, sizeof(Keyframe));
    if (!keyfr)
        APP_ERROR("Could not allocate memory.", 1);

    ServoPos *servo_pos = calloc(len, sizeof(ServoPos));
    if (!servo_pos)
        APP_ERROR("Could not allocate memory.", 1); 

    keyfr->servo_pos = servo_pos;

    double *trans_duration = (double *) config_get(CONF_TRANSITIONS_TIME);
    keyfr->duration = *trans_duration;    

    bool success = keyfactory_transition(keyfr, len, src, dest);

    if (!success)
    {
        if (keyfr)
            free(keyfr);
        keyfr = NULL;

        if (servo_pos)
            free(servo_pos);
        servo_pos = NULL;

        return;
    }

    list_push(&keyframes, (void *) keyfr);    
}

static void keyhandler_exec_removeall()
{
    Keyframe *keyfr_popped = NULL;
    keyfr_popped = (Keyframe *) list_pop(&keyframes);
    while (keyfr_popped != NULL)
    {
        if (keyfr_popped)
            free(keyfr_popped);    
        keyfr_popped = (Keyframe *) list_pop(&keyframes);
    }
    keyframes = NULL;
}

static void *keyhandler_main(void *arg)
{
    prctl(PR_SET_NAME, "PEABOT_KEYFR\0", NULL, NULL, NULL);

    struct timespec time;
    struct timespec last_time;
    double next = 0.0;
    
    Keyframe *keyfr;
    ServoPos *servo_pos;
    unsigned short *servos_num = (unsigned short *) config_get(CONF_SERVOS_NUM);
    
    double perc, pos, begin_time, end_time, adjusted_duration;

    while (running)
    {
        if (!keyframes)
        {
            next = 0.0;
            continue;       
        }

        if (exec_remove_all)
        {
            keyhandler_exec_removeall();
            exec_remove_all = false;
            continue;
        }

        clock_gettime(CLOCK_MONOTONIC, &time);
        next += utils_timediff(time, last_time);
        last_time = time;        

        keyfr = (Keyframe *) keyframes->data;
        
        if (keyfr->servo_pos != NULL)
            servo_pos = keyfr->servo_pos;
        else 
            servo_pos = NULL;

        if (!keyfr->is_delay && servo_pos)
        {
            for (unsigned short i = 0; i < *servos_num; i++)
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
            next = 0.0;
            keyhandler_log_keyfr(keyfr);

            if (servo_pos)
                free(servo_pos);
            servo_pos = NULL; 

            list_pop(&keyframes);


        }
    }

    return (void *) NULL;
}

static double keyhandler_mappos(double perc, ServoPos *servo_pos)
{
    double diff, modifier, delta, final;

    diff = servo_pos->end_pos - servo_pos->start_pos;
    modifier = easing_calc(servo_pos->easing, perc);
    delta = diff * modifier;    
    final = servo_pos->start_pos + delta;

    return final;
}

static void keyhandler_copy_keyfr(Keyframe *dest, Keyframe *src, size_t len)
{
    if (!dest || !src)
        return;

    dest->duration = src->duration;
    dest->is_delay = src->is_delay;
    
    if (src->servo_pos == NULL || dest->servo_pos == NULL)
    {
        dest->servo_pos = NULL;
        return;
    }

    for (unsigned short i = 0; i < len; i++)
        dest->servo_pos[i] = src->servo_pos[i];   
}

static void keyhandler_log_keyfr(Keyframe *keyfr)
{
    bool *log_keyframes = (bool *) config_get(CONF_LOG_KEYFRAMES);
    if (!*log_keyframes)
        return;

    char msg[LOG_LINE_MAXLEN];
    snprintf(msg, sizeof(msg), "[Keyfr] Completed keyframe. (duration: %f, is_delay: %s)", keyfr->duration, keyfr->is_delay ? "true" : "false");
    log_event(msg);
  
}

#endif