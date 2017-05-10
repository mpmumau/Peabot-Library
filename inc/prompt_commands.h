#ifndef PROMPT_COMMANDS_H_DEF
#define PROMPT_COMMANDS_H_DEF

/*
 File:          prompt_commands.h
 Description:   A listing of functions to be used as callbacks for prompt commands.
 Created:       May 9, 2017
 Author:        Matt Mumau
 */

/* Callback for resetting the robot. */
void promptcmd_reset(char *args[], int arg_num);

/* Callback for quitting the Peabot application */
void promptcmd_quit(char *args[], int arg_num);

/* Callback for delaying the event queue. */
void promptcmd_delay(char *args[], int arg_num);

/* Callback for moving a single servo motor. */
void promptcmd_srv(char *args[], int arg_num);

/* Callback for the robot's "up" operation */
void promptcmd_up(char *args[], int arg_num);

/* Callback for the "a" part of the robot's walk cycle. */
void promptcmd_walka(char *args[], int arg_num);

/* Callback for the "b" part of the robot's walk cycle. */
void promptcmd_walkb(char *args[], int arg_num);

/* Callback to sequence the robot for walking */
void promptcmd_walk(char *args[], int arg_num);

#endif