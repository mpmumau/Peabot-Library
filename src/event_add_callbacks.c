#ifndef EVENT_ADD_CALLBACKS_DEF
#define EVENT_ADD_CALLBACKS_DEF

/*
 File:          event_add_callbacks.h
 Description:   Definition of a variety of functions to be used for adding events. 
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdlib.h>

/* Application includes */
#include "event_handler.h"
#include "list.h"
#include "event_add_callbacks.h"

void eventadd_walka(List **events, float duration)
{
    Event *evt = malloc(sizeof(Event));
    
    evt->type = EVENT_WALK_A;
    evt->duration = duration;

    Keyframe *keyfr;
    keyfr_walka(&keyfr);
    evt->mvmts = keyfr;

    list_push(events, (void *) evt);   
}

void eventadd_walkb(List **events, float duration)
{
    Keyframe *keyfr;
    keyfr_walkb(&keyfr);

    Event *evt = malloc(sizeof(Event));
    evt->type = EVENT_WALK_B;
    evt->duration = duration;
    evt->mvmts = keyfr;

    list_push(events, (void *) evt);  
}

void eventadd_up(List **events, float duration)
{
    Event *evt = malloc(sizeof(Event));
    
    evt->type = EVENT_UP;
    evt->duration = duration;

    Keyframe *keyfr;
    keyfr_up(&keyfr);    

    Event *evt_trans = malloc(sizeof(Event));

    evt_trans->type = EVENT_TRANSITION;
    evt_trans->duration = 1.0f;

    Keyframe *trans_key;
    keyfr_transition(&trans_key, keyfr);

    evt_trans->mvmts = trans_key;

    list_push(events, (void *) evt_trans);    
    list_push(events, (void *) evt);  
}

#endif