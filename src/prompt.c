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

/* Command callbacks */
void promptcmd_reset(char *args[], int arg_num);
void promptcmd_quit(char *args[], int arg_num);
void promptcmd_delay(char *args[], int arg_num);
void promptcmd_srv(char *args[], int arg_num);
void promptcmd_up(char *args[], int arg_num);
void promptcmd_walka(char *args[], int arg_num);
void promptcmd_walkb(char *args[], int arg_num);
void promptcmd_walk(char *args[], int arg_num);

pthread_t prompt_thread;
char stdin_buffer[64];
bool prompt_running = true;

/* Event Callbacks */
static void (*cmd_callback)(char *args[], int args_num);

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

    char *tmp_arg;
    char delim[2] = " ";

    tmp_arg = strtok(stdin_str, delim);
    for (int i = 0; (i < arg_count) && tmp_arg; i++)
    {
        args[i] = tmp_arg;
        tmp_arg = strtok(NULL, delim);
    }    

    if (arg_count < 1)
        return;

    char *cmd = args[0];

    if (str_equals(cmd, "srv"))
        cmd_callback = promptcmd_srv;

    if (str_equals(cmd, "reset"))
        cmd_callback = promptcmd_reset;

    if (str_equals(cmd, "quit"))
        cmd_callback = promptcmd_quit;

    if (str_equals(cmd, "up"))
        cmd_callback = promptcmd_up;

    if (str_equals(cmd, "walka"))
        cmd_callback = promptcmd_walka;

    if (str_equals(cmd, "walkb"))
        cmd_callback = promptcmd_walkb;   

    if (str_equals(cmd, "walk"))
        cmd_callback = promptcmd_walk;

    if (str_equals(cmd, "delay"))
        cmd_callback = promptcmd_delay;     

    (*cmd_callback)(&args[1], arg_count);
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

/* Command callbacks */

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
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: delay [seconds]");
        return;
    }

    const char *seconds_string = args[1];
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
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: up [seconds]");
        return;
    }

    const char *seconds_string = args[1];
    float seconds = (float) atof(seconds_string);

    event_add(EVENT_UP, seconds);    
}

void promptcmd_walka(char *args[], int arg_num)
{
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: walka [seconds]");
    }

    const char *seconds_string = args[1];
    float seconds = (float) atof(seconds_string);

    event_add(EVENT_WALK_A, seconds);    
}

void promptcmd_walkb(char *args[], int arg_num)
{
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: walkb [seconds]");
    }

    const char *seconds_string = args[1];
    float seconds = (float) atof(seconds_string);

    event_add(EVENT_WALK_B, seconds);    
}

void promptcmd_walk(char *args[], int arg_num)
{
    if (arg_num != 3)
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

#endif