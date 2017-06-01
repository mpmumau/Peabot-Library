#ifndef EASING_UTILS_DEF
#define EASING_UTILS_DEF

/*
 File:          easing_utils.c
 Description:   Utility items for dealing with easing functions.
 Created:       May 12, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include "easing.h"

/* Header */
#include "easing_utils.h"

double easing_calc(unsigned short easing_type, AHFloat p)
{
    if (easing_type < 0)
        return (double) p;

    AHFloat (*easing_func)(AHFloat p);

    if (easing_type == EASE_LINEAR)
        easing_func = LinearInterpolation;


    if (easing_type == EASE_QUAD_IN)
        easing_func = QuadraticEaseIn;

    if (easing_type == EASE_QUAD_OUT)
        easing_func = QuadraticEaseOut;

    if (easing_type == EASE_QUAD_INOUT)
        easing_func = QuadraticEaseInOut;   


    if (easing_type == EASE_CUBE_IN)
        easing_func = CubicEaseIn;

    if (easing_type == EASE_CUBE_OUT)
        easing_func = CubicEaseOut;

    if (easing_type == EASE_CUBE_INOUT)
        easing_func = CubicEaseInOut;


    if (easing_type == EASE_QUART_IN)
        easing_func = QuarticEaseIn;

    if (easing_type == EASE_QUART_OUT)
        easing_func = QuarticEaseOut;

    if (easing_type == EASE_QUART_INOUT)
        easing_func = QuarticEaseInOut;


    if (easing_type == EASE_QUINT_IN)
        easing_func = QuinticEaseIn;

    if (easing_type == EASE_QUINT_OUT)
        easing_func = QuinticEaseOut;

    if (easing_type == EASE_QUINT_INOUT)
        easing_func = QuinticEaseInOut;     


    if (easing_type == EASE_SINE_IN)
        easing_func = SineEaseIn;

    if (easing_type == EASE_SINE_OUT)
        easing_func = SineEaseOut;

    if (easing_type == EASE_SINE_INOUT)
        easing_func = SineEaseInOut;    


    if (easing_type == EASE_CIRC_IN)
        easing_func = CircularEaseIn;       

    if (easing_type == EASE_CIRC_OUT)
        easing_func = CircularEaseOut;  

    if (easing_type == EASE_CIRC_INOUT)
        easing_func = CircularEaseInOut;    


    if (easing_type == EASE_EXP_IN)
        easing_func = ExponentialEaseIn;

    if (easing_type == EASE_EXP_OUT)
        easing_func = ExponentialEaseOut;

    if (easing_type == EASE_EXP_INOUT)
        easing_func = ExponentialEaseInOut; 


    if (easing_type == EASE_ELASTIC_IN)
        easing_func = ElasticEaseIn;    
        
    if (easing_type == EASE_ELASTIC_OUT)
        easing_func = ElasticEaseOut;   

    if (easing_type == EASE_ELASTIC_INOUT)
        easing_func = ElasticEaseInOut;     


    if (easing_type == EASE_BACK_IN)
        easing_func = BackEaseIn;       

    if (easing_type == EASE_BACK_OUT)
        easing_func = BackEaseOut;

    if (easing_type == EASE_BACK_INOUT)
        easing_func = BackEaseInOut;        


    if (easing_type == EASE_BOUNCE_IN)
        easing_func = BounceEaseIn;         

    if (easing_type == EASE_BOUNCE_OUT)
        easing_func = BounceEaseOut;    

    if (easing_type == EASE_BOUNCE_INOUT)
        easing_func = BounceEaseInOut;                              

    return (double) (*easing_func)(p);
}

#endif