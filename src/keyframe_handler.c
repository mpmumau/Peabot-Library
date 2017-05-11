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
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

/* Application includes */
#include "keyframe_handler.h"
#include "utils.h"
#include "list.h"
#include "easing.h"

static pthread_t keyhandler_thread;
static List *keyframes;
static bool running;
static Keyframe *last_keyfrm;  

/* Forward decs */
static void *keyhandler_main(void *arg);
static float keyhandler_getpos(float perc, ServoPos *servo_pos);
static bool keyhandler_checkdone(List *keyframes, float duration);

void keyhandler_init()
{
    pthread_create(&keyhandler_thread, NULL, keyhandler_main, NULL);
}

void keyhandler_halt()
{
    running = false;
    pthread_join(keyhandler_thread, NULL);
}

void keyhandler_add(int keyfr_type, void *data, bool reverse);
{
    Keyframe *(*keyhandler_add_cb)(void *data, bool reverse);
    Keyframe *keyfr;

    if (keyfr_type == KEYFR_RESET)
        keyhandler_add_cb = keyfradd_reset;

    if (keyfr_type == KEYFR_ELEVATE)
        keyhandler_add_cb = keyfradd_elevate;

    if (keyfr_type == KEYFR_WALK)
        keyhandler_add_cb = keyfradd_walk;

    if (*keyhandler_add_cb == NULL)
        return;
    keyfr = (*keyhandler_add_cb)(data, reverse);

    if (!keyfr)
        return;

    // Check if the new animation needs a transition keyframe and add it if so.
    Keyframe *current_keyfr;
    if (last_keyfrm)
        current_keyfr = last_keyfrm;
    else
        current_keyfr = keyfradd_home();

    Keyframe *trans_keyfr = keyfradd_transition(current_keyfr, keyfr);
    if (trans_keyfr)
        list_add(&keyframes, trans_keyfr);

    list_add(&keyframes, keyfr);
}

static void *keyhandler_main(void *arg)
{
    static struct timespec time;
    static struct timespec last_time;
    static float next = 0.0f;
    Keyframe *keyfr;

    while (running)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        next += utils_timediff(time, last_time);
        last_time = time;        

        if (!keyframes)
        {
            next = 0.0f;
            return;        
        }

        if (next > keyframes->duration)
        {
            next = 0.0f;

            if (last_keyframe && last_keyframe->servo_pos)
            {
                free(last_keyframe->servo_pos);
                free(last_keyframe);                
            }

            last_keyframe = (Keyframe *) list_pop(&keyframes);
            continue;
        }

        keyfr = (Keyframe *) keyframes->data;
        ServoPos *servo_pos = keyfr->servo_pos;
        float perc = next / keyfr->duration;
        float pos;

        for (int i = 0; i < SERVOS_NUM; i++)
        {
            pos = keyhandler_getpos(perc, servo_pos[i]);

            if (keyframe->reverse)
                pos *= -1.0f;

            robot_setservo(i, pos);
        }
    }
}

static float keyhandler_getpos(float perc, ServoPos *servo_pos)
{
    float diff, mod, delta, final;

    diff = servo_pos->end_pos - servo_pos->start_pos;
    mod = easing_calc(servo_pos->easing, perc);
    delta = diff * mod;
    final = servo_pos->start_pos + delta;

    return final;
}

static bool keyhandler_checkdone(List *keyframes, float duration)
{
    Keyframe *keyfr = (Keyframe *) keyframes->data;

    if (secs > event->duration)
        return true;

    return false;
}

#endif