#ifndef EVENT_HANDLER_H_DEF
#define EVENT_HANDLER_H_DEF

/*
 File:          event_handler.h
 Description:   Handle a queue of events and execute their actions.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

#define EVENT_RESET 0
#define EVENT_WALK_A 1
#define EVENT_WALK_B 2
#define EVENT_TURN 3
#define EVENT_DANCE 4
#define EVENT_UP 5

typedef struct Event {
    int type;
    float duration;
    struct servo_mvmt *mvmts;
} Event;

void event_tick();

void event_add(int event_type, float duration);

#endif