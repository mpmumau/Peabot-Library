#ifndef EVENT_CALLBACKS_H_DEF
#define EVENT_CALLBACKS_H_DEF

/*
 File:          event_callbacks.h
 Description:   Callback functions for events.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

/* Event to reset the robot to its home position. */
void eventcb_reset(void *arg);

/* Event to delay the robot's movements. */
void eventcb_delay(void *arg);

/* Event to do the robot's "elevate" animation. */
void eventcb_elevate(void *arg);

/* Event to do the robot's "extend" animation. */
void eventcb_extend(void *arg);

/* Event to do the robot's walk animation. */
void eventcb_walk(void *arg);

/* Event to start the robot's turn animation. */
void eventcb_turn(void *arg);

/* Event to stop all robot movement. */
void eventcb_halt(void *arg);

/* Event to move the robot laterally. */
void eventcb_strafe(void *arg);

#endif