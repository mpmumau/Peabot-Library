#ifndef KEYFRAME_FACTORY_DEF
#define KEYFRAME_FACTORY_DEF

/*
 File:          keyframe_factory.c
 Description:   Implementation of functions which create keyframe objects.
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Application includes */
#include "main.h"
#include "config_defaults.h"
#include "config.h"
#include "easing.h"
#include "easing_utils.h"
#include "keyframe_handler.h"

/* Header */
#include "keyframe_factory.h"

/* Forward decs */
static bool servopos_matches(ServoPos *src, ServoPos *dest, size_t len);
static unsigned short get_knee_from_leg(unsigned short leg);

bool keyfactory_reset(Keyframe *keyfr, size_t len, void *data, bool reverse)
{
    for (unsigned short i = 0; i < len; i++)
        keyfr->servo_pos[i] = (ServoPos) { -1, 0.0, 0.0, 0.0, 0.0 };

    return true;
}

bool keyfactory_delay(Keyframe *keyfr, size_t len, void *data, bool reverse)
{
    if (!data)
        return false;

    double *duration = (double *) data;
    keyfr->duration = *duration;

    keyfr->is_delay = true;

    return true;
}

bool keyfactory_elevate(Keyframe *keyfr, size_t len, void *data, bool reverse)
{
    if (!data)
        return false;

    double *duration = (double *) data;
    keyfr->duration = *duration;

    double mod = reverse ? -1.0 : 1.0;

    for (unsigned short i = 0; i < len; i++)
    {
        if (i == SERVO_INDEX_BACK_RIGHT_KNEE || 
            i == SERVO_INDEX_BACK_LEFT_KNEE || 
            i == SERVO_INDEX_FRONT_RIGHT_KNEE || 
            i == SERVO_INDEX_FRONT_LEFT_KNEE)

            keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, -mod, mod, 0.0, 0.0 };
        else
            keyfr->servo_pos[i] = (ServoPos) { -1, 0.0, 0.0, 0.0, 0.0 };
    }

    return true;
}

bool keyfactory_extend(Keyframe *keyfr, size_t len, void *data, bool reverse)
{
    if (!data)
        return false;

    double *duration = (double *) data;
    keyfr->duration = *duration;
    
    double mod = reverse ? -1.0 : 1.0;

    for (unsigned short i = 0; i < len; i++)
    {
        if (i == SERVO_INDEX_BACK_RIGHT_HIP || 
            i == SERVO_INDEX_BACK_LEFT_HIP || 
            i == SERVO_INDEX_FRONT_RIGHT_HIP || 
            i == SERVO_INDEX_FRONT_LEFT_HIP)

            keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, -mod, mod, 0.0, 0.0 };
        else
            keyfr->servo_pos[i] = (ServoPos) { -1, 0.0, 0.0, 0.0, 0.0 };
    }

    return true;
}

bool keyfactory_walk(Keyframe *keyfr, size_t len, void *data, bool reverse)
{
    if (!data)
        return false;

    static bool is_inverted = true;

    double *duration = (double *) data;
    keyfr->duration = *duration;

    double *knee_delta = (double *) config_get(CONF_WALK_KNEE_DELTA);
    double *hip_delta = (double *) config_get(CONF_WALK_HIP_DELTA);
    double *knee_pad_a = (double *) config_get(CONF_WALK_KNEE_PAD_A);
    double *knee_pad_b = (double *) config_get(CONF_WALK_KNEE_PAD_B);

    double knee_pad_ax = is_inverted ? *knee_pad_a : *knee_pad_b;
    double knee_pad_bx = is_inverted ? *knee_pad_b : *knee_pad_a;

    unsigned short ease_in = is_inverted ? EASE_CIRC_IN : EASE_CIRC_OUT;
    unsigned short ease_out = is_inverted ? EASE_CIRC_OUT : EASE_CIRC_IN;

    double knee_bottom = 0.8;

    double mod = is_inverted ? -1.0 : 1.0;

    mod *= reverse ? -1.0 : 1.0;

    for (unsigned short i = 0; i < len; i++)
    {
        keyfr->servo_pos[i] = (ServoPos) { -1, 0.0, 0.0, 0.0, 0.0 };

        if (i == SERVO_INDEX_BACK_LEFT_HIP || i == SERVO_INDEX_FRONT_LEFT_HIP)
            keyfr->servo_pos[i] = (ServoPos) { i == SERVO_INDEX_BACK_LEFT_HIP ? ease_out : ease_in, *hip_delta * mod, -(*hip_delta) * mod, 0.0, 0.0 };

        if (i == SERVO_INDEX_BACK_RIGHT_HIP || i == SERVO_INDEX_FRONT_RIGHT_HIP)
            keyfr->servo_pos[i] = (ServoPos) { i == SERVO_INDEX_BACK_RIGHT_HIP ? ease_in : ease_out, -(*hip_delta) * mod, *hip_delta * mod, 0.0, 0.0 };

        if (!is_inverted)
        {
            if (i == SERVO_INDEX_BACK_LEFT_KNEE || i == SERVO_INDEX_FRONT_RIGHT_KNEE)
                keyfr->servo_pos[i] = (ServoPos) { ease_out, knee_bottom, -*knee_delta, knee_pad_bx, 0.0 };
            if (i == SERVO_INDEX_FRONT_LEFT_KNEE || i == SERVO_INDEX_BACK_RIGHT_KNEE)
                keyfr->servo_pos[i] = (ServoPos) { ease_out, -*knee_delta, knee_bottom, knee_pad_ax, 0.0 };
        }
        else
        {
            if (i == SERVO_INDEX_BACK_LEFT_KNEE || i == SERVO_INDEX_FRONT_RIGHT_KNEE)
                keyfr->servo_pos[i] = (ServoPos) { ease_out, -*knee_delta, knee_bottom, knee_pad_bx, 0.0 };
            if (i == SERVO_INDEX_FRONT_LEFT_KNEE || i == SERVO_INDEX_BACK_RIGHT_KNEE)
                keyfr->servo_pos[i] = (ServoPos) { ease_out, knee_bottom, -*knee_delta, knee_pad_ax, 0.0 };
        }
    }

    is_inverted = !is_inverted;

    return true;
}

bool keyfactory_turnsegment(Keyframe *keyfr, size_t len, void *data, bool reverse)
{
    if (!data)
        return false;

    double *duration = (double *) data;
    keyfr->duration = *duration;

    static unsigned short leg = SERVO_INDEX_FRONT_RIGHT_HIP;
    static bool legs_complete[4];
    unsigned short knee = get_knee_from_leg(leg);
    
    bool all_complete = true;
    for (unsigned short x = 0; x < 4; x++)
    {
        if (legs_complete[x] == false)
        {
            all_complete = false;
            break;
        }
    }

    if (all_complete)
    {
        for (unsigned short e = 0; e < 4; e++)
        {
            legs_complete[e] = false;
        }
        leg = SERVO_INDEX_FRONT_LEFT_HIP;
    }

    double turn_delta = 0.9 * (reverse ? -1.0 : 1.0);
    double knee_delta = 0.4;

    if (!all_complete)
    {
        for (int i = 0; i < len; i++)
        {
            if (i == SERVO_INDEX_BACK_RIGHT_HIP || 
                i == SERVO_INDEX_FRONT_RIGHT_HIP ||
                i == SERVO_INDEX_BACK_LEFT_HIP ||
                i == SERVO_INDEX_FRONT_LEFT_HIP)
            {
                if (i == leg)
                {
                    keyfr->servo_pos[knee] = (ServoPos) { EASE_SINE_IN, knee_delta, 1.0, 0.0, 0.0 };
                    if (i == SERVO_INDEX_BACK_RIGHT_HIP || i == SERVO_INDEX_FRONT_LEFT_HIP)
                        keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, 0.0, -turn_delta, 0.0, 0.0 };    
                    else 
                        keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, 0.0, turn_delta, 0.0, 0.0 }; 
                }
                else
                {
                    switch (i)
                    {
                        case SERVO_INDEX_FRONT_RIGHT_HIP:
                            if (legs_complete[0] == true)
                                keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, turn_delta, turn_delta, 0.0, 0.0 };
                            else
                                keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, 0.0, 0.0, 0.0, 0.0 };    
                            break;
                        case SERVO_INDEX_BACK_LEFT_HIP:
                            if (legs_complete[3] == true)
                                keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, turn_delta, turn_delta, 0.0, 0.0 };
                            else
                                keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, 0.0f, 0.0, 0.0, 0.0 };    
                            break;
                        case SERVO_INDEX_BACK_RIGHT_HIP:
                            if (legs_complete[1] == true)
                                keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, -turn_delta, -turn_delta, 0.0, 0.0 };
                            else
                                keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, 0.0, 0.0, 0.0, 0.0 };    
                            break;
                        case SERVO_INDEX_FRONT_LEFT_HIP:
                            if (legs_complete[2] == true)
                                keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, -turn_delta, -turn_delta, 0.0, 0.0 };
                            else
                                keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, 0.0, 0.0, 0.0, 0.0 };    
                            break;
                    }
                }
            }
            else
            {
                if (i != knee)
                {
                    keyfr->servo_pos[i] = (ServoPos) { -1, 1.0, 1.0, 0.0, 0.0 };
                }
            }       
        }
    }   
    else
    {
        for (unsigned int i = 0; i < len; i++)
        {
            if (i == SERVO_INDEX_BACK_RIGHT_HIP || 
                i == SERVO_INDEX_FRONT_RIGHT_HIP ||
                i == SERVO_INDEX_BACK_LEFT_HIP ||
                i == SERVO_INDEX_FRONT_LEFT_HIP)
            {
                    if (i == SERVO_INDEX_BACK_RIGHT_HIP || i == SERVO_INDEX_FRONT_LEFT_HIP)
                    {
                        keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, -turn_delta, 0.0, 0.0, 0.0 };    
                    }
                    else 
                    {
                        keyfr->servo_pos[i] = (ServoPos) { EASE_SINE_IN, turn_delta, 0.0, 0.0, 0.0 }; 
                    }
            }
            else
            {
                keyfr->servo_pos[i] = (ServoPos) { -1, 1.0, 1.0, 0.0, 0.0 };
            }            
        }
    }

    if (!all_complete)
    {
        switch (leg)
        {
            case SERVO_INDEX_FRONT_RIGHT_HIP:
                legs_complete[0] = true;
                break;
            case SERVO_INDEX_BACK_RIGHT_HIP:
                legs_complete[1] = true;
                break;
            case SERVO_INDEX_FRONT_LEFT_HIP:
                legs_complete[2] = true;
                break;
            case SERVO_INDEX_BACK_LEFT_HIP:
                legs_complete[3] = true;
                break;
        }
    }    

    switch(leg)
    {
        case SERVO_INDEX_FRONT_RIGHT_HIP:
            if (reverse)
                leg = SERVO_INDEX_FRONT_LEFT_HIP;
            else
                leg = SERVO_INDEX_BACK_RIGHT_HIP;
            break;
        case SERVO_INDEX_BACK_RIGHT_HIP:
            if (reverse)
                leg = SERVO_INDEX_FRONT_RIGHT_HIP;
            else
                leg = SERVO_INDEX_BACK_LEFT_HIP;
            break;
        case SERVO_INDEX_BACK_LEFT_HIP:
            if (reverse)
                leg = SERVO_INDEX_BACK_RIGHT_HIP;
            else
                leg = SERVO_INDEX_FRONT_LEFT_HIP;
            break;
        case SERVO_INDEX_FRONT_LEFT_HIP:
            if (reverse)
                leg = SERVO_INDEX_BACK_LEFT_HIP;
            else
                leg = SERVO_INDEX_FRONT_RIGHT_HIP;
            break;
    }    

    return true;
}

bool keyfactory_transition(Keyframe *keyfr, size_t len, Keyframe *src, Keyframe *dest)
{
    printf("Making transition keyfr...\n");

    printf("-----TRANS SRC KEYFR-----\n");
    keyhandler_print_keyfr(src, len);

    printf("-----TRANS DEST KEYFR-----\n");
    keyhandler_print_keyfr(dest, len);    

    if (src == NULL || dest == NULL)
        return false;

    if (src->servo_pos == NULL || dest->servo_pos == NULL)
        return false;

    if (servopos_matches(src->servo_pos, dest->servo_pos, len))
        return false;

    for (unsigned short i = 0; i < len; i++)
    {
        keyfr->servo_pos[i] = (ServoPos) { EASE_CIRC_IN, src->servo_pos[i].end_pos, dest->servo_pos[i].start_pos, 0.0, 0.0 };
    }

    printf("-----TRANS KEYFR-----\n");
    keyhandler_print_keyfr(keyfr, len);     
    
    return true; 
}

static bool servopos_matches(ServoPos *src, ServoPos *dest, size_t len)
{
    if (!src || !dest)
        return true;

    printf("=====servopos_matches()=====\n");
    for (unsigned short i = 0; i < len; i++) 
    {
        printf("Comparing: src.end_pos[%f] to dest.start_pos[%f]\n", src[i].end_pos, dest[i].start_pos);
        if (src[i].end_pos != dest[i].start_pos)
            return false;  

        src++;
        dest++;
    }

    return true;
}

static unsigned short get_knee_from_leg(unsigned short leg)
{
    switch (leg)
    {
        case SERVO_INDEX_FRONT_RIGHT_HIP:
            return SERVO_INDEX_FRONT_RIGHT_KNEE;
        case SERVO_INDEX_BACK_RIGHT_HIP:
            return SERVO_INDEX_BACK_RIGHT_KNEE;
        case SERVO_INDEX_FRONT_LEFT_HIP:
            return SERVO_INDEX_FRONT_LEFT_KNEE;
        case SERVO_INDEX_BACK_LEFT_HIP:
            return SERVO_INDEX_BACK_LEFT_KNEE;
        default:
            return -1;
    }
}

#endif