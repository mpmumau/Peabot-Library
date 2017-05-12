#ifndef PROMPT_COMMANDS_DEF
#define PROMPT_COMMANDS_DEF

/*
 File:          prompt_commands.c
 Description:   Implementation of prompt command callback functions.
 Created:       May 9, 2017
 Author:        Matt Mumau
 */

/* System includes */ 
#include <stdlib.h>

/* Application includes */
#include "main.h"
#include "console.h"
#include "event_handler.h"

/* Header */
#include "prompt_commands.h"

void promptcmd_quit(char *args[], int arg_num)
{
    app_exit("[Prompt] User requested application shutdown.", 0);
}


void promptcmd_reset(char *args[], int arg_num)
{
    event_add(EVENT_RESET, (void *) NULL);  
}

void promptcmd_delay(char *args[], int arg_num)
{
    if (arg_num != 1)
    {
        console_print("[ERROR] Incorrect number of params. Usage: delay [seconds]");
        return;
    }

    const char *seconds_string = args[0];
    float *seconds = malloc(sizeof(float));
    *seconds = (float) atof(seconds_string);

    event_add(EVENT_DELAY, (void *) seconds);    
}

void promptcmd_elevate(char *args[], int arg_num)
{
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: up [seconds] [reverse]");
        return;
    }

    const char *seconds_string = args[0];
    const char *reverse_string = args[1];

    EventElevateData *elevate_data = malloc(sizeof(EventElevateData));
    elevate_data->reverse = (bool) ((int) atoi(reverse_string));
    elevate_data->duration = (float) atof(seconds_string);

    event_add(EVENT_UP, (void *) elevate_data);    
}

void promptcmd_walk(char *args[], int arg_num)
{
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: walk [cycles] [cycle_secs/2]");
        return;
    }

    const char *cycles_string = args[0];
    const char *seconds_string = args[1];

    EventWalkData *walk_data = malloc(sizeof(EventWalkData));
    walk_data->cycles = atoi(cycles_string);
    walk_data->duration = atof(seconds_string);

    event_add(EVENT_WALK, (void *) walk_data);
}

#endif