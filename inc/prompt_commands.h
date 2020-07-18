/*
 File:          prompt_commands.h
 Description:   A listing of functions to be used as callbacks for prompt commands.
 */

#ifndef PROMPT_COMMANDS_H_DEF
#define PROMPT_COMMANDS_H_DEF

/* Callback for quitting the Peabot application */
void promptcmd_quit(char *args[], int arg_num);

/* Callback for printing a config variable's value to the console. */
void promptcmd_cfg_get(char *args[], int arg_num);

#endif