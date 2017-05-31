#ifndef EVENT_H_DEF
#define EVENT_H_DEF

/*
 File:          events.h
 Description:   Handle a queue of events and execute their actions.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

#include <stdbool.h>

#define EVENT_RESET 0
#define EVENT_HALT 1
#define EVENT_DELAY 2
#define EVENT_ELEVATE 3
#define EVENT_WALK 4
#define EVENT_EXTEND 5
#define EVENT_TURN 6

typedef struct Event {
    int type;
    void *data;
} Event;

typedef struct EventElevateData {
    bool reverse;
    double duration;
} EventElevateData;

typedef struct EventExtendData {
    bool reverse;
    double duration;
} EventExtendData;

typedef struct EventWalkData {
    unsigned short cycles;
    double duration;
    bool reverse;
} EventWalkData;

typedef struct EventTurnData {
    unsigned short cycles;
    double duration;
    bool reverse;
} EventTurnData;

/* Initialize the event handler thread. */
void event_init();

/* End the event handler thread and stop processing events. */
void event_halt();

/* Add an event and to the event queue. */
void event_add(unsigned short event_type, void *data);

#endif