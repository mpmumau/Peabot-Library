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

static Keyframe last_keyfr;
static ServoPos last_servopos;

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
        keyhandler_add_transition(*servos_num, &last_keyfr, keyfr);

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

    printf("Adding transition keyframe... (no: %d)\n", list_sizeof(keyframes));

    list_push(&keyframes, (void *) keyfr);    

    printf("Done. Added transition keyframe (no: %d)\n", list_sizeof(keyframes));
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

    unsigned short *servos_num = (unsigned short *) config_get(CONF_SERVOS_NUM);

    last_keyfr.duration = 0.0;
    last_keyfr.is_delay = false;
    last_keyfr.servo_pos = &last_servopos;    

    for (int q = 0; q < *servos_num; q++)
    {
        last_keyfr.servo_pos[q].easing = -1;
        last_keyfr.servo_pos[q].start_pos = 0.0;
        last_keyfr.servo_pos[q].end_pos = 0.0;
        last_keyfr.servo_pos[q].begin_pad = 0.0;
        last_keyfr.servo_pos[q].end_pad = 0.0;
    }

    struct timespec time;
    struct timespec last_time;
    double next = 0.0;
    
    Keyframe *keyfr;
    Keyframe *tmp_key;
    ServoPos *servo_pos;
    
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

                // todo: remove
                //printf("begin_time: %f, end_time: %f, adjusted_duration: %f, perc: %f, pos: %f\n", begin_time, end_time, adjusted_duration, perc, pos);
            }
        }

        if (next > keyfr->duration)
        {
            next = 0.0;
            keyhandler_log_keyfr(keyfr);

            if (servo_pos)
                free(servo_pos);
            servo_pos = NULL; 

            tmp_key = (Keyframe *) list_pop(&keyframes);
            if (tmp_key)
                free(tmp_key);       
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

    // todo: remove
    //printf("diff: %f, modifier: %f, delta: %f, final: %f\n", diff, modifier, delta, final);

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

void keyhandler_print_keyfr(Keyframe *keyfr, size_t len)
{
    if (!keyfr)
        return;

    printf("keyfr->duration: %f\n", keyfr->duration);
    printf("keyfr->is_delay: %s\n", keyfr->is_delay ? "true" : "false");

    if (keyfr->servo_pos)
    {
        printf("keyfr->servo_pos:\n");
        for (int i = 0; i < len; i++)
        {
            printf("\t[%d] easing: %d, \tstart_pos: %6.6f, \t\tend_pos: %6.6f, \t\tbegin_pad: %6.6f, \t\tend_pad: %6.6f\n", 
                i, keyfr->servo_pos[i].easing, keyfr->servo_pos[i].start_pos, keyfr->servo_pos[i].end_pos, keyfr->servo_pos[i].begin_pad, keyfr->servo_pos[i].end_pad);
        }
    }   
    else
    {
        printf("keyfr->servo_pos: NULL\n");
    }
}

#endif