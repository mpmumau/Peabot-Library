#ifndef KEYFRAME_H_DEF
#define KEYFRAME_H_DEF

/*
 File:          keyframe.h
 Description:   A data structure and utility functions for handling servo keyframes.
 Created:       May 8, 2017
 Author:        Matt Mumau
 Note:          The following functions all take a pointer to a Keyframe pointer, 
                which will be set to the value of a new pointer, representing an
                animation keyframe, in which the positions of all servos are set.
 */

#include "config.h"

#ifndef SERVOS_NUM
#define SERVOS_NUM 8
#endif

#define KEYFR_RESET 0;
#define KEYFR_ELEVATE 1;
#define KEYFR_WALK 2;

typedef struct ServoPos {
    int easing;
    float start_pos;
    float end_pos;
    float delay; 
    float stop;   
} ServoPos;

/* Data structure for representing servo positions at a point in time. */
typedef struct Keyframe {
    float duration;
    ServoPos servo_pos[SERVOS_NUM];
} Keyframe;

#endif