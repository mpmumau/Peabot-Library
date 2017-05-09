#ifndef MOVEMENTS_DEF
#define MOVEMENTS_DEF

/*
 File:          movements.c
 Description:   Implementation of a data structure and utility functions for servo sweeps.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

/* System includes*/
#include <stdlib.h>

/* Application includes */
#include "config.h"

#include "movements.h"

struct servo_mvmt mvmt_walk_a[SERVOS_NUM] = {
    { .start_pos = -0.2f, .end_pos = 0.2f, .delay = 0.0f },
    { .start_pos = -0.2f, .end_pos = 0.2f, .delay = 0.0f },
    { .start_pos = -0.2f, .end_pos = 0.2f, .delay = 0.0f },
    { .start_pos = -0.2f, .end_pos = 0.2f, .delay = 0.0f },
    { .start_pos = -0.2f, .end_pos = 0.2f, .delay = 0.0f },
    { .start_pos = -0.2f, .end_pos = 0.2f, .delay = 0.0f },
    { .start_pos = -0.2f, .end_pos = 0.2f, .delay = 0.0f },
    { .start_pos = -0.2f, .end_pos = 0.2f, .delay = 0.0f },
}

#endif