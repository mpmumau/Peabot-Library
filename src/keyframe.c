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

Keyframe keyfr_walka[SERVOS_NUM] = {
    { 0.4f, 0.4f, 0.0f},
    { 0.6f, -0.6f, 0.0f },
    { 0.6f, -0.6f, 0.0f },
    { -0.4, 0.4, 0.0f},
    { -0.4, 0.4, 0.0f },
    { -0.6f, 0.6f, 0.0f },
    { 0.4f, 0.4f, 0.0f},   
    { -0.6f, 0.6f, 0.0f }
};

Keyframe keyfr_walkb[SERVOS_NUM] = {
    { -0.6, 0.6, 0.0f},
    { -0.6f, 0.6f, 0.0f },
    { -0.6f, 0.6f, 0.0f },
    {},
    {},
    { 0.6f, -0.6f, 0.0f },
    { -0.6, 0.6, 0.0f},    
    { 0.6f, -0.6f, 0.0f }
};

Keyframe keyfr_up[SERVOS_NUM] = {
    { -1.0, 1.0, 0.0f },
    {},
    {},
    { -1.0, 1.0, 0.0f },
    { -1.0, 1.0, 0.0f },
    {},
    { -1.0, 1.0, 0.0f },
    {}
};

#endif