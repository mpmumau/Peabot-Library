#ifndef MOVEMENTS_H_DEF
#define MOVEMENTS_H_DEF

/*
 File:          movements.h
 Description:   A data structure and utility functions for handling servo sweeps.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

struct servo_mvmt {
    float start_pos;
    float end_pos;
    float delay;
};

#endif