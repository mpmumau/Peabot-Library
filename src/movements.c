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
    { -0.7f, 0.7f, 0.0f },
    { -0.7f, 0.7f, 0.0f },
    {  0.7f, -0.7f, 0.0f },
    { -0.2f, 0.2f, 0.0f },
    { -0.2f, 0.2f, 0.0f },
    { -0.2f, 0.2f, 0.0f },
    { -0.2f, 0.2f, 0.0f },    
    { -0.2f, 0.2f, 0.0f }
};

struct servo_mvmt mvmt_up[SERVOS_NUM] = {
    { -1.0, 1.0, 0.0f },
    NULL,
    NULL,
    { -1.0, 1.0, 0.0f },
    { -1.0, 1.0, 0.0f },
    NULL,
    { -1.0, 1.0, 0.0f },
    NULL
}

#endif