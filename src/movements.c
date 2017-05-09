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

struct servo_mvmt *mvmt_walk_a()
{
    struct servo_mvmt *mvmt = malloc(sizeof(struct servo_mvmt) * SERVOS_NUM);

    for (int i = 0; i < SERVOS_NUM; i++)
    {
        mvmt->start_pos = -0.2f;
        mvmt->end_pos = 0.2f;
        mvmt->delay = 1.1f;

        mvmt++; 
    }

    return mvmt[0];
}

#endif