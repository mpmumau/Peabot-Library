#ifndef PROMPT_COMMANDS_H_DEF
#define PROMPT_COMMANDS_H_DEF

/*
 File:          prompt_commands.h
 Description:   A listing of functions to be used as callbacks for prompt commands.
 Created:       May 9, 2017
 Author:        Matt Mumau
 */

/* Callback for quitting the Peabot application */
void promptcmd_quit(char *args[], int arg_num);

/* Callback for resetting the robot. */
void promptcmd_reset(char *args[], int arg_num);

/* Callback for delaying the event queue. */
void promptcmd_delay(char *args[], int arg_num);

/* Callback for the robot's "up" operation */
void promptcmd_elevate(char *args[], int arg_num);

/* Callback for the robot's "extend" operation. */
void promptcmd_extend(char *args[], int arg_num);

/* Callback to sequence the robot for walking */
void promptcmd_walk(char *args[], int arg_num);

/* Callback for printing a config variable's value to the console. */
void promptcmd_cfg_get(char *args[], int arg_num);

/* Callback for doing the robot's turn animation. */
void promptcmd_turn(char *args[], int arg_num);

void promptcmd_halt(char *args[], int arg_num);

#endif