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
#include "prompt_commands.h"
#include "console.h"
#include "string_utils.h"
#include "log.h"

/* Forward decs */
static void *prompt_main(void *arg);
static void prompt_handle_cmd(char *stdin);
static int prompt_count_args(char *arg_str);

static pthread_t prompt_thread;
static bool prompt_running = true;

void prompt_init()
{
    pthread_create(&prompt_thread, NULL, prompt_main, NULL);
}

void prompt_halt()
{
    prompt_running = false;
    pthread_join(prompt_thread, NULL);
}

static void *prompt_main(void *arg)
{
    char stdin_buffer[64];

    while (prompt_running)
    {
        printf("peabot > ");
        fgets(stdin_buffer, sizeof(stdin_buffer), stdin);
        
        str_removenl(stdin_buffer);

        prompt_handle_cmd(stdin_buffer);
    }

    return (void *) NULL;
}

sttic void prompt_handle_cmd(char *stdin_str)
{
    if (LOG_STDIN)
    {
        char ancmt[64];
        snprintf(ancmt, 64, "[Stdin]: %s", stdin_str);
        log_event(ancmt);
    }

    int arg_count = prompt_count_args(stdin_str);
    char *args[arg_count];

    void (*cmd_callback)(char *args[], int args_num);

    char *tmp_arg;
    char delim[2] = { ' ', '\0'};

    tmp_arg = strtok(stdin_str, delim);
    for (int i = 0; (i < arg_count) && tmp_arg; i++)
    {
        args[i] = tmp_arg;
        tmp_arg = strtok(NULL, delim);
    }    

    if (arg_count < 1)
        return;
    arg_count--;

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

static int prompt_count_args(char *arg_str)
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