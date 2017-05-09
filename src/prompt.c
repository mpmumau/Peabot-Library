#ifndef PROMPT_DEF
#define PROMPT_DEF

/*
 File:          prompt.c
 Description:   Implementation of functions related to the command prompt.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

/* Application includes */
#include "config.h"
#include "prompt.h"
#include "robot.h"
#include "console.h"
#include "string_utils.h"
#include "log.h"
#include "main.h"
#include "event_handler.h"

/* Forward decs */
void *prompt_tick(void *arg);
void prompt_handle_cmd(char *stdin);
int prompt_count_args(char *arg_str);
void prompt_cmd_srv(char *args[]);

pthread_t prompt_thread;
char stdin_buffer[64];
bool prompt_running = true;

/* Event Callbacks */
static void (*_cmd_srv)(char *args[]) = prompt_cmd_srv;

void prompt_init()
{
    pthread_create(&prompt_thread, NULL, prompt_tick, NULL);
}

void *prompt_tick(void *arg)
{
    while (prompt_running)
    {
        printf("peabot > ");
        fgets(stdin_buffer, sizeof(stdin_buffer), stdin);
        
        str_removenl(stdin_buffer);

        prompt_handle_cmd(stdin_buffer);
    }

    return (void *) NULL;
}

void prompt_halt()
{
    prompt_running = false;
    pthread_join(prompt_thread, NULL);
}

void prompt_handle_cmd(char *stdin_str)
{
    if (LOG_STDIN)
    {
        char ancmt[64];
        snprintf(ancmt, 64, "[Stdin]: %s", stdin_str);
        log_event(ancmt);
    }
    
    int arg_count = prompt_count_args(stdin_str);
    
    char *args[arg_count];

    char *arg;
    char delim[2] = " ";

    arg = strtok(stdin_str, delim);
    for (int i = 0; (i < arg_count) && arg; i++)
    {
        args[i] = arg;
        arg = strtok(NULL, delim);
    }    

    if (arg_count < 1)
        return;

    if (str_equals(args[0], "srv"))
        (*_cmd_srv)(args[1]);

    if (str_equals(args[0], "reset"))
    {
        robot_reset();
    }

    if (str_equals(args[0], "quit"))
    {
        app_exit("User requested application shutdown.", 0);
    }

    if (str_equals(args[0], "up"))
    {
        if (arg_count != 2)
        {
            console_print("[ERROR] Incorrect number of params. Usage: up [seconds]");
        }

        const char *seconds_string = args[1];
        float seconds = (float) atof(seconds_string);

        event_add(EVENT_UP, seconds);
    }

    if (str_equals(args[0], "walka"))
    {
        if (arg_count != 2)
        {
            console_print("[ERROR] Incorrect number of params. Usage: walka [seconds]");
        }

        const char *seconds_string = args[1];
        float seconds = (float) atof(seconds_string);

        event_add(EVENT_WALK_A, seconds);
    }

    if (str_equals(args[0], "walkb"))
    {
        if (arg_count != 2)
        {
            console_print("[ERROR] Incorrect number of params. Usage: walkb [seconds]");
        }

        const char *seconds_string = args[1];
        float seconds = (float) atof(seconds_string);

        event_add(EVENT_WALK_B, seconds);
    }    

    if (str_equals(args[0], "walk"))
    {
        if (arg_count != 3)
        {
            console_print("[ERROR] Incorrect number of params. Usage: walk [cycles] [cycle_secs/2]");
        }

        const char *cycles_string = args[1];
        const char *seconds_string = args[2];

        int cycles = (int) atoi(cycles_string);
        float seconds = (float) atof(seconds_string);

        for (int i = 0; i < cycles; i++)
        {
            event_add(EVENT_WALK_A, seconds);
            event_add(EVENT_WALK_B, seconds);
        }
    }

    if (str_equals(args[0], "delay"))
    {
        if (arg_count != 2)
        {
            console_print("[ERROR] Incorrect number of params. Usage: delay [seconds]");
        }

        const char *seconds_string = args[1];
        float seconds = (float) atof(seconds_string);

        event_add(EVENT_DELAY, seconds);
    }        
}

void prompt_cmd_srv(char *args[])
{
    const char *s_pin = args[0];
    const char *s_val = args[1];

    int pin = (int) atoi(s_pin);
    float val = (float) atof(s_val);

    robot_setservo(pin, val);
}

int prompt_count_args(char *arg_str)
{
    char tmp_str[64];

    str_copy(tmp_str, arg_str);

    int arg_count = 0;
    char *arg;
    char delim[2] = " ";

    arg = strtok(tmp_str, delim);
    while (arg)
    {
        arg_count++;
        arg = strtok(NULL, delim);
    }   

    return arg_count;
}

#endif