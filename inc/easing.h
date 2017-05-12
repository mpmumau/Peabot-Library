//
//  easing.h
//
//  Copyright (c) 2011, Auerhaus Development, LLC
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What The Fuck You Want
//  To Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/wtfpl/COPYING for more details.
//

#ifndef AH_EASING_H
#define AH_EASING_H

#if defined(__LP64__) && !defined(AH_EASING_USE_DBL_PRECIS)
#define AH_EASING_USE_DBL_PRECIS
#endif

#ifdef AH_EASING_USE_DBL_PRECIS
#define AH_FLOAT_TYPE double
#else
#define AH_FLOAT_TYPE float
#endif
typedef AH_FLOAT_TYPE AHFloat;

#if defined __cplusplus
extern "C" {
#endif

typedef AHFloat (*AHEasingFunction)(AHFloat);

// Linear interpolation (no easing)
AHFloat LinearInterpolation(AHFloat p);
#define EASE_LINEAR 0

// Quadratic easing; p^2
AHFloat QuadraticEaseIn(AHFloat p);
AHFloat QuadraticEaseOut(AHFloat p);
AHFloat QuadraticEaseInOut(AHFloat p);
#define EASE_QUAD_IN 1
#define EASE_QUAD_OUT 2
#define EASE_QUAD_INOUT 3

// Cubic easing; p^3
AHFloat CubicEaseIn(AHFloat p);
AHFloat CubicEaseOut(AHFloat p);
AHFloat CubicEaseInOut(AHFloat p);
#define EASE_CUBE_IN 4
#define EASE_CUBE_OUT 5
#define EASE_CUBE_INOUT 6

// Quartic easing; p^4
AHFloat QuarticEaseIn(AHFloat p);
AHFloat QuarticEaseOut(AHFloat p);
AHFloat QuarticEaseInOut(AHFloat p);
#define EASE_QUART_IN 7
#define EASE_QUART_OUT 8
#define EASE_QUART_INOUT 9

// Quintic easing; p^5
AHFloat QuinticEaseIn(AHFloat p);
AHFloat QuinticEaseOut(AHFloat p);
AHFloat QuinticEaseInOut(AHFloat p);
#define EASE_QUINT_IN 10
#define EASE_QUINT_OUT 11
#define EASE_QUINT_INOUT 12

// Sine wave easing; sin(p * PI/2)
AHFloat SineEaseIn(AHFloat p);
AHFloat SineEaseOut(AHFloat p);
AHFloat SineEaseInOut(AHFloat p);
#define EASE_SINE_IN 13
#define EASE_SINE_OUT 14
#define EASE_SINE_INOUT 15

// Circular easing; sqrt(1 - p^2)
AHFloat CircularEaseIn(AHFloat p);
AHFloat CircularEaseOut(AHFloat p);
AHFloat CircularEaseInOut(AHFloat p);
#define EASE_CIRC_IN 16
#define EASE_CIRC_OUT 17
#define EASE_CIRC_INOUT 18

// Exponential easing, base 2
AHFloat ExponentialEaseIn(AHFloat p);
AHFloat ExponentialEaseOut(AHFloat p);
AHFloat ExponentialEaseInOut(AHFloat p);
#define EASE_EXP_IN 19
#define EASE_EXP_OUT 20
#define EASE_EXP_INOUT 21

// Exponentially-damped sine wave easing
AHFloat ElasticEaseIn(AHFloat p);
AHFloat ElasticEaseOut(AHFloat p);
AHFloat ElasticEaseInOut(AHFloat p);
#define EASE_ELASTIC_IN 22
#define EASE_ELASTIC_OUT 23
#define EASE_ELASTIC_INOUT 24

// Overshooting cubic easing; 
AHFloat BackEaseIn(AHFloat p);
AHFloat BackEaseOut(AHFloat p);
AHFloat BackEaseInOut(AHFloat p);
#define EASE_BACK_IN 25
#define EASE_BACK_OUT 26
#define EASE_BACK_INOUT 27

// Exponentially-decaying bounce easing
AHFloat BounceEaseIn(AHFloat p);
AHFloat BounceEaseOut(AHFloat p);
AHFloat BounceEaseInOut(AHFloat p);
#define EASE_BOUNCE_IN 28
#define EASE_BOUNCE_OUT 29
#define EASE_BOUNCE_INOUT 30

#ifdef __cplusplus
}
#endif

float easing_calc(int easing_type, AHFloat p);

#endif
