#ifndef PROMPT
#define PROMPT

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

pthread_t prompt_thread;
char stdin_buffer[64];
bool prompt_running = true;

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
    {
        if (arg_count != 3)
        {
            console_print("[ERROR] Incorrect number of params. Usage: srv [pin_num] [pwm_val]");
        }
            
        const char *servo_pin_string = args[1];
        const char *servo_val_string = args[2];

        int servo_pin = (int) atoi(servo_pin_string);
        float pwm_val = (float) atof(servo_val_string);

        robot_setservo(servo_pin, pwm_val);
    }

    if (str_equals(args[0], "reset"))
    {
        robot_reset();
    }

    if (str_equals(args[0], "quit"))
    {
        app_exit("User requested application shutdown.", 0);
    }

    if (str_equals(args[0], "test_event"))
    {
        event_add(0, 0.0f);
        event_add(1, 1.0f);
        event_add(2, 2.0f);
        event_add(3, 4.0f);
        event_add(4, 8.0f);
    }
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