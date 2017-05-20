#ifndef KEYFRAME_FACTORY_H_DEF
#define KEYFRAME_FACTORY_H_DEF

/*
 File:          keyframe_factory.h
 Description:   Definitions of functions for creating keyframe objects.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

#include "keyframe_handler.h"

/* Get keyframe for resetting the robot to the home position. */
Keyframe *keyfactory_home(void *data, bool reverse);

/* Adds a delay keyframe, which bypasses servo movement for the given duration. */
Keyframe *keyfactory_delay(void *data, bool reverse);

/* Get keyframe to move all the robot's knees to the fully extended position. */
Keyframe *keyfactory_elevate(void *data, bool reverse);

/* Get a keyframe for extending or retracting the hip servos. */
Keyframe *keyfactory_extend(void *data, bool reverse);

/* Get keyframe to add a walk animation. */
Keyframe *keyfactory_walk(void *data, bool reverse);

/* Get a keyframe for a transition between the src and dest ServoPos objects. */
Keyframe *keyfactory_transition(void *data, bool reverse);

#endif