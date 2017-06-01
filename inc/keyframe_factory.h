#ifndef KEYFRAME_FACTORY_H_DEF
#define KEYFRAME_FACTORY_H_DEF

/*
 File:          keyframe_factory.h
 Description:   Definitions of functions for creating keyframe objects.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

#include <stdbool.h>

#include "keyframe_handler.h"

/* Get keyframe for resetting the robot to the home position. */
bool keyfactory_reset(Keyframe *keyfr, size_t len, void *data, bool reverse);

/* Adds a delay keyframe, which bypasses servo movement for the given duration. */
bool keyfactory_delay(Keyframe *keyfr, size_t len, void *data, bool reverse);

/* Get keyframe to move all the robot's knees to the fully extended position. */
bool keyfactory_elevate(Keyframe *keyfr, size_t len, void *data, bool reverse);

/* Get a keyframe for extending or retracting the hip servos. */
bool keyfactory_extend(Keyframe *keyfr, size_t len, void *data, bool reverse);

/* Get keyframe to add a walk animation. */
bool keyfactory_walk(Keyframe *keyfr, size_t len, void *data, bool reverse);

/* Get a single segment of a turn animation (one leg). */
bool keyfactory_turnsegment(Keyframe *keyfr, size_t len, void *data, bool reverse);

/* Get a keyframe for a transition between the src and dest ServoPos objects. */
bool keyfactory_transition(Keyframe *keyfr, size_t len, Keyframe *src, Keyframe *dest);

#endif