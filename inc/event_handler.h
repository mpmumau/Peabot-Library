#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

/*
 File:          event_handler.h
 Description:   Handle a queue of events and execute their associated acctions.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

#define EVENT_WALK 0
#define EVENT_RESET 1
#define EVENT_TURN 2
#define EVENT_DANCE 3

struct event {
    int type;
};

void event_tick();

void event_add(int event_type);

#endif