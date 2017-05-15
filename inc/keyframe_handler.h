#ifndef KEYFRAME_HANDLER_H_DEF
#define KEYFRAME_HANDLER_H_DEF

/*
 File:          keyframe_handler.h
 Description:   Handler for keyframe animation processing. 
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

#include <stdbool.h>

#ifndef SERVOS_NUM
#define SERVOS_NUM 8
#endif

#define KEYFR_HOME 0
#define KEYFR_DELAY 1
#define KEYFR_ELEVATE 2
#define KEYFR_WALK 3

typedef struct ServoPos {
    int easing;
    float start_pos;
    float end_pos;
    float begin_pad; 
    float end_pad;   
} ServoPos;

/* Data structure for representing servo positions at a point in time. */
typedef struct Keyframe {
    float duration;
    bool is_delay;
    ServoPos *servo_pos;
} Keyframe;

typedef struct KeyframeTransData {
    float duration;    
    ServoPos *src;
    ServoPos *dest;
} KeyframeTransData;

/* Initialize the keyframe handler process. */
void keyhandler_init();

/* End the keyframe process thread and stop processing keyframes. */
void keyhandler_halt();

/* Add a keyframe to the keyframe queue. */
void keyhandler_add(int keyfr_type, void *data, bool reverse, bool skip_transitions);

#endif