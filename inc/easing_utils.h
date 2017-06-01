#ifndef EASING_UTILS_H_DEF
#define EASING_UTILS_H_DEF

/*
 File:          easing_utils.h
 Description:   Utility items for dealing with easing functions.
 Created:       May 12, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include "easing.h"

#define EASE_LINEAR 0

#define EASE_QUAD_IN 1
#define EASE_QUAD_OUT 2
#define EASE_QUAD_INOUT 3

#define EASE_CUBE_IN 4
#define EASE_CUBE_OUT 5
#define EASE_CUBE_INOUT 6

#define EASE_QUART_IN 7
#define EASE_QUART_OUT 8
#define EASE_QUART_INOUT 9

#define EASE_QUINT_IN 10
#define EASE_QUINT_OUT 11
#define EASE_QUINT_INOUT 12

#define EASE_SINE_IN 13
#define EASE_SINE_OUT 14
#define EASE_SINE_INOUT 15

#define EASE_CIRC_IN 16
#define EASE_CIRC_OUT 17
#define EASE_CIRC_INOUT 18

#define EASE_EXP_IN 19
#define EASE_EXP_OUT 20
#define EASE_EXP_INOUT 21

#define EASE_ELASTIC_IN 22
#define EASE_ELASTIC_OUT 23
#define EASE_ELASTIC_INOUT 24

#define EASE_BACK_IN 25
#define EASE_BACK_OUT 26
#define EASE_BACK_INOUT 27

#define EASE_BOUNCE_IN 28
#define EASE_BOUNCE_OUT 29
#define EASE_BOUNCE_INOUT 30

double easing_calc(unsigned short easing_type, AHFloat p);

#endif