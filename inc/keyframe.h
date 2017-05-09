#ifndef KEYFRAME_H_DEF
#define KEYFRAME_H_DEF

/*
 File:          keyframe.h
 Description:   A data structure and utility functions for handling servo keyframes.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

#include "config.h"

typedef struct Keyframe {
    float start_pos;
    float end_pos;
    float delay;
} Keyframe;

Keyframe keyfr_walka[SERVOS_NUM];
Keyframe keyfr_up[SERVOS_NUM];

#endif