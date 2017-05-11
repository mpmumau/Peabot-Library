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

/* Data structure for representing servo positions at a point in time. */
typedef struct Keyframe {
    float start_pos;
    float end_pos;
    float delay;
} Keyframe;

/* The "Up" robot motion, in which all the robot's knees are retracted, then extended. */
void keyfr_up(Keyframe **keyfr);

/* The first segment of the robot's walk animation. */
void keyfr_walka(Keyframe **keyfr);

/* The second segment of the robot's walk animation. */
void keyfr_walkb(Keyframe **keyfr);

/* 
 * Create a keyframe between the robot's current position, and the target keyframe's
 * start position. 
 */
void keyfr_transition(Keyframe **keyfrm, Keyframe *target);

#endif