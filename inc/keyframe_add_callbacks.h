#ifndef KEYFRAME_ADD_CALLBACKS_H_DEF
#define KEYFRAME_ADD_CALLBACKS_H_DEF

/*
 File:          keyframe_add_callbacks.h
 Description:   Definitions of functions for creating keyframe objects.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

/* Get keyframe for resetting the robot to the home position. */
Keyframe *keyfradd_home();

/* Get keyframe to move all the robot's knees to the fully extended position. */
Keyframe *keyfradd_elevate(void *data, bool reverse);

/* Get keyframe to add a walk animation. */
Keyframe *keyfradd_walk(void *data, bool reverse);

/* Get a keyframe for a transition between the src and dest ServoPos objects. */
Keyframe *keyfradd_transition(ServoPos *src, ServoPos *dest);

#endif