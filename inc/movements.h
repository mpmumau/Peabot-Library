#ifndef MOVEMENTS_H_DEF
#define MOVEMENTS_H_DEF

/*
 File:          movements.h
 Description:   A data structure and utility functions for handling servo sweeps.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

#include "config.h"

typedef struct Keyframe {
    float start_pos;
    float end_pos;
    float delay;
} Keyframe;

Keyframe mvmt_walk_a[SERVOS_NUM];
Keyframe mvmt_up[SERVOS_NUM];

#endif