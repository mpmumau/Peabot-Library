#ifndef CONTROLLER_EVENT_H_DEF
#define CONTROLLER_EVENT_H_DEF

/*
 File:          controller_event.h
 Description:   Controller function for HTTP event models.
 Created:       May 28, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdbool.h>

/* Application includes */
#include "mvc_data.h"

bool cntlevent_walk(MVCData *mvc_data);

bool cntlevent_turn(MVCData *mvc_data);

bool cntlevent_elevate(MVCData *mvc_data);

bool cntlevent_extend(MVCData *mvc_data);

bool cntlevent_delay(MVCData *mvc_data);

bool cntlevent_reset(MVCData *mvc_data);

bool cntlevent_halt(MVCData *mvc_data);

bool cntlevent_strafe(MVCData *mvc_data);

#endif