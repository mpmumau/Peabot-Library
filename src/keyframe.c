#ifndef KEYFRAME_DEF
#define KEYFRAME_DEF

/*
 File:          keyframe.c
 Description:   Implementation of a data structure and utility functions for servo keyframes.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

/* System includes*/
#include <stdlib.h>

/* Application includes */
#include "config.h"

#include "keyframe.h"

void keyfr_walka(Keyframe **keyfr)
{
    Keyframe *tmp_keyfr = malloc(sizeof(Keyframe) * SERVOS_NUM);

    tmp_keyfr[0] = (Keyframe) { 0.4f, 0.4f, 0.0f};
    tmp_keyfr[1] = (Keyframe) { 0.6f, -0.6f, 0.0f };
    tmp_keyfr[2] = (Keyframe) { 0.6f, -0.6f, 0.0f };
    tmp_keyfr[3] = (Keyframe) { -0.4, 0.4, 0.0f};
    tmp_keyfr[4] = (Keyframe) { -0.4, 0.4, 0.0f };
    tmp_keyfr[5] = (Keyframe) { -0.6f, 0.6f, 0.0f };
    tmp_keyfr[6] = (Keyframe) { 0.4f, 0.4f, 0.0f}; 
    tmp_keyfr[7] = (Keyframe) { -0.6f, 0.6f, 0.0f };

    (*keyfr) = tmp_keyfr;
}

void keyfr_walkb(Keyframe **keyfr)
{
    Keyframe *tmp_keyfr = malloc(sizeof(Keyframe) * SERVOS_NUM);

    tmp_keyfr[0] = (Keyframe) { -0.4, 0.4, 0.0f };
    tmp_keyfr[1] = (Keyframe) { -0.6f, 0.6f, 0.0f };
    tmp_keyfr[2] = (Keyframe) { -0.6f, 0.6f, 0.0f };
    tmp_keyfr[3] = (Keyframe) { 0.4f, 0.4f, 0.0f };
    tmp_keyfr[4] = (Keyframe) { 0.4f, 0.4f, 0.0f };
    tmp_keyfr[5] = (Keyframe) { 0.6f, -0.6f, 0.0f };
    tmp_keyfr[6] = (Keyframe) { -0.6, 0.6, 0.0f};
    tmp_keyfr[7] = (Keyframe) { 0.4f, -0.4f, 0.0f };

    (*keyfr) = tmp_keyfr;
}

void keyfr_up(Keyframe **keyfr)
{
    Keyframe *tmp_keyfr = malloc(sizeof(Keyframe) * SERVOS_NUM);

    tmp_keyfr[0] = (Keyframe) { -1.0, 1.0, 0.0f };
    tmp_keyfr[3] = (Keyframe) { -1.0, 1.0, 0.0f };
    tmp_keyfr[4] = (Keyframe) { -1.0, 1.0, 0.0f };
    tmp_keyfr[6] = (Keyframe) { -1.0, 1.0, 0.0f };

    (*keyfr) = tmp_keyfr;
}

void keyfrm_transition(Keyframe **keyfrm, Keyframe *target)
{
    Keyframe *tmp = malloc(sizeof(Keyframe) * SERVOS_NUM);

    for (int i = 0; i < SERVOS_NUM; i++)
        tmp[i] = (Keyframe) { robot_getservo(i), target->end_pos, 0.0f };

    *keyfrm = tmp;
}

#endif