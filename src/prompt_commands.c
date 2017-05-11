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
#include "robot.h"

/* Header */
#include "prompt_commands.h"

void promptcmd_reset(char *args[], int arg_num)
{
    robot_reset();    
}

void promptcmd_quit(char *args[], int arg_num)
{
    app_exit("User requested application shutdown.", 0);
}

void promptcmd_delay(char *args[], int arg_num)
{
    if (arg_num != 1)
    {
        console_print("[ERROR] Incorrect number of params. Usage: delay [seconds]");
        return;
    }

    const char *seconds_string = args[0];
    float seconds = (float) atof(seconds_string);

    event_add(EVENT_DELAY, seconds);    
}

void promptcmd_srv(char *args[], int arg_num)
{
    if (arg_num != 2)
    {
        console_print("ERROR] Incorrect number of params. Usage: srv [pin] [pwm_val]");
        return;
    }

    const char *s_pin = args[0];
    const char *s_val = args[1];

    int pin = (int) atoi((s_pin));
    float val = (float) atof(s_val);

    robot_setservo(pin, val);
}

void promptcmd_up(char *args[], int arg_num)
{
    if (arg_num != 1)
    {
        console_print("[ERROR] Incorrect number of params. Usage: up [seconds]");
        return;
    }

    const char *seconds_string = args[0];
    float seconds = (float) atof(seconds_string);

    event_add(EVENT_UP, seconds);    
}

void promptcmd_walka(char *args[], int arg_num)
{
    if (arg_num != 1)
    {
        console_print("[ERROR] Incorrect number of params. Usage: walka [seconds]");
        return;
    }

    const char *seconds_string = args[0];
    float seconds = (float) atof(seconds_string);

    event_add(EVENT_WALK_A, seconds);    
}

void promptcmd_walkb(char *args[], int arg_num)
{
    if (arg_num != 1)
    {
        console_print("[ERROR] Incorrect number of params. Usage: walkb [seconds]");
        return;
    }

    const char *seconds_string = args[0];
    float seconds = (float) atof(seconds_string);

    event_add(EVENT_WALK_B, seconds);    
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

    int cycles = (int) atoi(cycles_string);
    float seconds = (float) atof(seconds_string);

    for (int i = 0; i < cycles; i++)
    {
        event_add(EVENT_WALK_A, seconds);
        event_add(EVENT_WALK_B, seconds);
    }    
}

#endif