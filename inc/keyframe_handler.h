#ifndef KEYFRAME_HANDLER_H_DEF
#define KEYFRAME_HANDLER_H_DEF

/*
 File:          keyframe_handler.h
 Description:   Handler for keyframe animation processing. 
 Created:       May 11, 2017
 Author:        Matt Mumau
 */

/* Initialize the keyframe handler process. */
void keyhandler_init()

/* End the keyframe process thread and stop processing keyframes. */
void keyhandler_halt();

/* Add a keyframe to the keyframe queue. */
void keyhandler_add(int keyfr);

#endif