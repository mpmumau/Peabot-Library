#ifndef KEYFRAME_HANDLER_H_DEF
#define KEYFRAME_HANDLER_H_DEF

/*
 File:          keyframe_handler.h
 Description:   Handler for keyframe animation processing. 
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

#include "config.h"

#ifndef SERVOS_NUM
#define SERVOS_NUM 8
#endif

#define KEYFR_RESET 0;
#define KEYFR_ELEVATE 1;
#define KEYFR_WALK 2;

typedef struct ServoPos {
    int easing;
    float start_pos;
    float end_pos;
    float delay; 
    float stop;   
} ServoPos;

/* Data structure for representing servo positions at a point in time. */
typedef struct Keyframe {
    float duration;
    ServoPos servo_pos[SERVOS_NUM];
} Keyframe;

/* Initialize the keyframe handler process. */
void keyhandler_init()

/* End the keyframe process thread and stop processing keyframes. */
void keyhandler_halt();

/* Add a keyframe to the keyframe queue. */
void keyhandler_add(int keyfr);

#endif