#ifndef KEYFRAME_FACTORY_DEF
#define KEYFRAME_FACTORY_DEF

/*
 File:          keyframe_factory.c
 Description:   Implementation of functions which create keyframe objects.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdlib.h>

/* Application includes */
#include "main.h"
#include "config_defaults.h"
#include "config.h"
#include "easing.h"
#include "easing_utils.h"
#include "keyframe_handler.h"

/* Header */
#include "keyframe_factory.h"

/* Forward decs */
static bool servopos_matches(ServoPos *src, ServoPos *dest);

Keyframe *keyfactory_home(void *data, bool reverse)
{
    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);

    ServoPos *servo_pos = malloc(sizeof(ServoPos) * (*servos_num));
    if (!servo_pos)
        app_exit("[ERROR!] Failed to allocate memory for servo_pos (keyfradd_home).", 1);

    for (int i = 0; i < *servos_num; i++)
    {
        servo_pos[i] = (ServoPos) { -1, 0.0f, 0.0f, 0.0f, 0.0f };
    }

    Keyframe *keyfr = malloc(sizeof(Keyframe));
    if (!keyfr)
        app_exit("[ERROR!] Failed to allocate memory for keyfr (keyfradd_home).", 1);

    keyfr->servo_pos = servo_pos;

    return keyfr;
}

Keyframe *keyfactory_delay(void *data, bool reverse)
{
    float *duration = (float *) data;

    Keyframe *keyfr = malloc(sizeof(Keyframe));
    if (!keyfr)
        app_exit("[ERROR!] Failed to allocate memory for keyfr (keyfradd_delay).", 1);

    keyfr->duration = *duration;
    keyfr->is_delay = true;

    return keyfr;
}

Keyframe *keyfactory_elevate(void *data, bool reverse)
{
    float *duration = (float *) data;
    float mod = reverse ? -1.0f : 1.0f;

    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);

    ServoPos *servo_pos = malloc(sizeof(ServoPos) * (*servos_num));
    if (!servo_pos)
        app_exit("[ERROR] Could not allocate memory for ServoPos. (keyfradd_elevate)", 1);

    float start_pos = -1.0f * mod;
    float end_pos = 1.0f * mod;

    for (int i = 0; i < *servos_num; i++)
    {
        if (i == SERVO_INDEX_BACK_RIGHT_KNEE || 
            i == SERVO_INDEX_BACK_LEFT_KNEE || 
            i == SERVO_INDEX_FRONT_RIGHT_KNEE || 
            i == SERVO_INDEX_FRONT_LEFT_KNEE)

            servo_pos[i] = (ServoPos) { EASE_SINE_IN, start_pos, end_pos, 0.0f, 0.0f };
        else
            servo_pos[i] = (ServoPos) { -1, 0.0f, 0.0f, 0.0f, 0.0f };
    }

    Keyframe *keyfr = malloc(sizeof(Keyframe));
    if (!keyfr)
        app_exit("[ERROR] Could not allocate memory for keyframe. (keyfradd_elevate)", 1);

    keyfr->duration = *duration;
    keyfr->servo_pos = servo_pos;

    return keyfr;
}

Keyframe *keyfactory_walk(void *data, bool reverse)
{
    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);

    ServoPos *servo_pos = malloc(sizeof(ServoPos) * *servos_num);
    if (!servo_pos)
        app_exit("[ERROR] Could not allocate memory for ServoPos (keyfradd_walk).", 1);

    float *duration = (float *) data;

    float *knee_delta = (float *) config_get(CONF_WALK_KNEE_DELTA);
    float *hip_delta = (float *) config_get(CONF_WALK_HIP_DELTA);

    float *knee_pad_a = (float *) config_get(CONF_WALK_KNEE_PAD_A);
    float *knee_pad_b = (float *) config_get(CONF_WALK_KNEE_PAD_B);

    float knee_pad_ax = reverse ? *knee_pad_a : *knee_pad_b;
    float knee_pad_bx = reverse ? *knee_pad_b : *knee_pad_a;

    int ease_in = reverse ? EASE_CIRC_IN : EASE_CIRC_OUT;
    int ease_out = reverse ? EASE_CIRC_OUT : EASE_CIRC_IN;

    for (int i = 0; i < *servos_num; i++)
    {
        // tmp debug
        servo_pos[i] = (ServoPos) { -1, 0.0f, 0.0f, 0.0f, 0.0f };

        if (i == SERVO_INDEX_BACK_LEFT_HIP || i == SERVO_INDEX_FRONT_LEFT_HIP)
            servo_pos[i] = (ServoPos) { ease_in, -*hip_delta, *hip_delta, 0.0f, 0.0f };

        // if (i == SERVO_INDEX_BACK_RIGHT_HIP || i == SERVO_INDEX_FRONT_RIGHT_HIP)
        //     servo_pos[i] = (ServoPos) { ease_out, *hip_delta, -*hip_delta, 0.0f, 0.0f };

        // if (!reverse)
        // {
        //     if (i == SERVO_INDEX_BACK_LEFT_KNEE || i == SERVO_INDEX_FRONT_RIGHT_KNEE)
        //         servo_pos[i] = (ServoPos) { ease_out, -*knee_delta, *knee_delta, knee_pad_bx, 0.0f };

        //     if (i == SERVO_INDEX_FRONT_LEFT_KNEE || i == SERVO_INDEX_BACK_RIGHT_KNEE)
        //         servo_pos[i] = (ServoPos) { ease_out, *knee_delta, -*knee_delta, knee_pad_ax, 0.0f };
        // }
        // else
        // {
        //     if (i == SERVO_INDEX_BACK_LEFT_KNEE || i == SERVO_INDEX_FRONT_RIGHT_KNEE)
        //         servo_pos[i] = (ServoPos) { ease_out, *knee_delta, -*knee_delta, knee_pad_bx, 0.0f };

        //     if (i == SERVO_INDEX_FRONT_LEFT_KNEE || i == SERVO_INDEX_BACK_RIGHT_KNEE)
        //         servo_pos[i] = (ServoPos) { ease_out, -*knee_delta, *knee_delta, knee_pad_ax, 0.0f };
        // }
    }

    Keyframe *keyfr = malloc(sizeof(Keyframe));
    if (!keyfr)
        app_exit("[ERROR] Could not allocate memory for keyframe (keyfradd_walk).", 1);

    keyfr->duration = *duration;
    keyfr->servo_pos = servo_pos;

    return keyfr;
}

Keyframe *keyfactory_transition(void *data, bool reverse)
{
    if (!data)
        return NULL;

    KeyframeTransData *trans_data = (KeyframeTransData *) data;

    ServoPos *src = trans_data->src;
    ServoPos *dest = trans_data->dest;

    float duration = trans_data->duration;

    if (servopos_matches(src, dest))
        return NULL;

    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);

    ServoPos *servo_pos = malloc(sizeof(ServoPos) * (*servos_num));
    if (!servo_pos)
        app_exit("[ERROR] Could not allocate memory for servo_pos (keyfradd_transition).", 1);



    for (int i = 0; i < *servos_num; i++)
    {
        servo_pos[i] = (ServoPos) { EASE_CIRC_IN, src->end_pos, dest->start_pos, 0.0f, 0.0f };
        src++;
        dest++;
    }

    Keyframe *keyfr = malloc(sizeof(Keyframe));
    if (!keyfr)
        app_exit("[ERROR] Could not allocate memory for keyfr (keyfradd_transition).", 1);

    keyfr->duration = duration;
    keyfr->servo_pos = servo_pos;
    
    return keyfr; 
}

static bool servopos_matches(ServoPos *src, ServoPos *dest)
{
    bool matches = true;

    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);

    for (int i = 0; i < *servos_num; i++) 
    {
        if (src->end_pos != dest->start_pos)
        {
            matches = false;
            break;   
        }

        src++;
        dest++;
    }

    return matches;
}

#endif