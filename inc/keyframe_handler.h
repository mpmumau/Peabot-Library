#ifndef KEYFRAME_HANDLER_H_DEF
#define KEYFRAME_HANDLER_H_DEF

/*
 File:          keyframe_handler.h
 Description:   Handler for keyframe animation processing. 
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

#include <stdbool.h>

#define KEYFR_RESET 0
#define KEYFR_DELAY 1
#define KEYFR_ELEVATE 2
#define KEYFR_WALK 3
#define KEYFR_EXTEND 4
#define KEYFR_TURN 5
#define KEYFR_STRAFE 6

typedef struct ServoPos {
    unsigned short easing;
    double start_pos;
    double end_pos;
    double begin_pad; 
    double end_pad;   
} ServoPos;

/* Data structure for representing servo positions at a point in time. */
typedef struct Keyframe {
    double duration;
    bool is_delay;
    ServoPos *servo_pos;
} Keyframe;

/* Initialize the keyframe handler process. */
void keyhandler_init();

/* End the keyframe process thread and stop processing keyframes. */
void keyhandler_halt();

/* Add a keyframe to the keyframe queue. */
void keyhandler_add(unsigned short keyfr_type, void *data, bool reverse, bool skip_transitions);

void keyhandler_removeall();

void keyhandler_print_keyfr(Keyframe *keyfr, size_t len);

#endif