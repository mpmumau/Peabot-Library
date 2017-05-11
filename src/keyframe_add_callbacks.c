#ifndef KEYFRAME_ADD_CALLBACKS_DEF
#define KEYFRAME_ADD_CALLBACKS_DEF

/*
 File:          keyframe_add_callbacks.c
 Description:   Implementation of functions that add keyframe objects.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */


// typedef struct ServoPos {
//     int easing;
//     float start_pos;
//     float end_pos;
//     float delay; 
//     float stop;   
// } ServoPos;

//  Data structure for representing servo positions at a point in time. 
// typedef struct Keyframe {
//     float duration;
//     ServoPos servo_pos[SERVOS_NUM];
//     bool reverse;
// } Keyframe;


#include "main.h"
#include "keyframe_add_callbacks.h"

/* Forward decs */
static bool keyfradd_servopos_matches(ServoPos *src, ServoPos *dest);

Keyframe *keyfradd_home()
{
    ServoPos *servo_pos = malloc(sizeof(ServoPos) * SERVOS_NUM);

    for (int i = 0; i < SERVOS_NUM; i++)
    {
        servo_pos[i] = (ServoPos) { NULL, 0.0f, 0.0f, 0.0f, 0.0f };
    }

    Keyframe *keyfr = malloc(sizeof(ServoPos));
    keyfr->duration = 0.0f;
    keyfr->servos_pos = servo_pos;

    return keyfr;
}

Keyframe *keyfradd_elevate(void *data, bool reverse)
{
    float duration = (float) data;
    float mod = reverse ? -1.0f : 1.0f;

    ServoPos *servo_pos = malloc(sizeof(ServoPos) * SERVOS_NUM);
    if (!servo_pos)
        app_exit("[ERROR] Could not allocate memory for ServoPos.", 1);

    float start_pos = -1.0f * mod;
    float end_pos = 1.0f * mod;

    for (int i = 0; i < SERVOS_NUM; i++)
    {
        if (i == BACK_RIGHT_KNEE || i == BACK_LEFT_KNEE || i == FRONT_RIGHT_KNEE || i == FRONT_LEFT_KNEE)
            servo_pos[i] = (ServoPos) { EASE_SINE_IN, start_pos, end_pos, 0.0f, 0.0f };
        else
            servo_pos[i] = (ServoPos) { NULL, 0.0f, 0.0f, 0.0f, 0.0f };
    }

    Keyframe *keyfr = malloc(sizeof(Keyframe));
    if (!keyfr)
        app_exit("[ERROR] Could not allocate memory for keyframe.", 1);

    keyfr->duration = duration;
    keyfr->servo_pos = servo_pos;

    return keyfr;
}

Keyframe *keyfradd_walk(void *data, bool reverse)
{
    float duration = (float) data;

    ServoPos *servo_pos = malloc(sizeof(ServoPos) * SERVOS_NUM);
    if (!servo_pos)
        app_exit("[ERROR] Could not allocate memory for ServoPos.", 1);

    servo_pos[BACK_RIGHT_HIP] = (ServoPos) { EASE_CIRC_OUT, -0.6f, 0.6f, 0.0f, 0.0f };
    servo_pos[BACK_RIGHT_KNEE] = (ServoPos) { EASE_CIRC_OUT, -0.4f, 0.4f, 0.0f, 0.0f };
    servo_pos[BACK_LEFT_HIP] = (ServoPos) { EASE_CIRC_IN, 0.6f, -0.6f, 0.0f, 0.0f };
    servo_pos[BACK_LEFT_KNEE] = (ServoPos) { EASE_CIRC_IN, 0.4f, 0.4f, 0.0f, 0.0f };
    servo_pos[FRONT_RIGHT_HIP] = (ServoPos) { EASE_CIRC_IN, -0.6f, 0.6f, 0.0f, 0.0f };
    servo_pos[FRONT_RIGHT_KNEE] = (ServoPos) { EASE_CIRC_IN, 0.4f, 0.4f, 0.0f, 0.0f }; 
    servo_pos[FRONT_LEFT_HIP] = (ServoPos) { EASE_CIRC_OUT, 0.6f, -0.6f, 0.0f, 0.0f };
    servo_pos[FRONT_LEFT_KNEE] = (ServoPos) { EASE_CIRC_OUT, -0.4f, 0.4f, 0.0f, 0.0f };    

    Keyframe *keyfr = malloc(sizeof(Keyframe));
    if (!keyfr)
        app_exit("[ERROR] Could not allocate memory for keyframe.", 1);

    keyfr->duration = duration;
    keyfr->servo_pos = servo_pos;

    return keyfr;
}

Keyframe *keyfradd_transition(ServoPos *src, ServoPos *dest)
{
    if (!src || !dest)
        return NULL;

    if (keyfradd_servopos_matches(src, dest))
        return NULL;

    ServoPos *servo_pos = malloc(sizeof(ServoPos) * SERVOS_NUM);
    for (int i = 0; i < SERVOS_NUM; i++)
    {
        servo_pos[i] = (ServoPos) { EASE_CIRC_IN, src->end_pos, dest->start_pos, 0.0f, 0.0f };
    }

    Keyframe *keyfr = malloc(sizeof(Keyframe));
    keyfr->duration = 2.0f;
    keyfr->servo_pos = servo_pos;
    keyfr->reverse = false;

    return keyfr; 
}

static bool keyfradd_servopos_matches(ServoPos *src, ServoPos *dest)
{
    bool matches = true;

    for (int i = 0; i < SERVOS_NUM; i++)
    {
        if (src[i]->end_pos != dest[i]->start_pos)
            matches = false;
    }

    return matches;
}