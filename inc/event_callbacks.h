#ifndef EVENT_CALLBACKS_H_DEF
#define EVENT_CALLBACKS_H_DEF

/*
 File:          event_callbacks.h
 Description:   Callback functions for events.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

void eventcb_reset(void *arg);

void eventcb_delay(void *arg);

void eventcb_elevate(void *arg);

void eventcb_walk(void *arg);

#endif