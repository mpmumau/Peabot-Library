#ifndef PROMPT_DEF
#define PROMPT_DEF

/*
 File:          prompt.c
 Description:   Implementation of functions related to the command prompt.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <sys/prctl.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

/* Application includes */
#include "main.h"
#include "config.h"
#include "log.h"
#include "console.h"
#include "prompt_commands.h"
#include "string_utils.h"

/* Header */
#include "prompt.h"

/* Forward decs */
static void *prompt_main(void *arg);
static void prompt_handle_cmd(const char *stdin_str, size_t len);
static int prompt_count_args(char *arg_str, size_t len);
static void prompt_log_stdin(const char *str);

static pthread_t prompt_thread;
static bool running = true;

void prompt_init()
{
    pthread_attr_t prompt_thread_attr;
    pthread_attr_init(&prompt_thread_attr);
    pthread_attr_setdetachstate(&prompt_thread_attr, PTHREAD_CREATE_DETACHED);

    int error = pthread_create(&prompt_thread, &prompt_thread_attr, prompt_main, NULL);
    if (error)
        APP_ERROR("Could not initialize prompt thread.", error);

    pthread_attr_destroy(&prompt_thread_attr);
}

void prompt_halt()
{
    running = false;
}

void prompt_print()
{
    printf("peabot > ");
}

static void *prompt_main(void *arg)
{
    prctl(PR_SET_NAME, "PEABOT_PROMPT\0", NULL, NULL, NULL);

    char stdin_buffer[STDIN_BUFFER_LEN]; 

    while (running)
    {
        prompt_print();
        fgets(stdin_buffer, sizeof(stdin_buffer), stdin);
        str_removenl(stdin_buffer);
        prompt_log_stdin(stdin_buffer);
        prompt_handle_cmd(stdin_buffer, sizeof(stdin_buffer));
    }

    pthread_exit(NULL);
    return (void *) NULL;
}

static void prompt_handle_cmd(const char *stdin_str, size_t len)
{
    int arg_count = prompt_count_args(stdin_str, len);
    char *args[arg_count];

    void (*cmd_callback)(char *args[], int args_num) = NULL;

    char *tmp_arg;
    char delim[2] = { ' ', '\0' };

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

    if (str_equals(cmd, "quit"))
        cmd_callback = promptcmd_quit;

    if (str_equals(cmd, "reset"))
        cmd_callback = promptcmd_reset;

    if (str_equals(cmd, "delay"))
        cmd_callback = promptcmd_delay;      

    if (str_equals(cmd, "elevate"))
        cmd_callback = promptcmd_elevate;

    if (str_equals(cmd, "extend"))
        cmd_callback = promptcmd_extend;

    if (str_equals(cmd, "walk"))
        cmd_callback = promptcmd_walk;

    if (str_equals(cmd, "cfg_get"))
        cmd_callback = promptcmd_cfg_get;

    if (str_equals(cmd, "turn"))
        cmd_callback = promptcmd_turn;

    if (str_equals(cmd, "halt"))
        cmd_callback = promptcmd_halt;    

    if (cmd_callback == NULL)
    {
        console_print("[ERROR] Unknown command.");
        return;
    }

    (*cmd_callback)(&args[1], arg_count);
}

static int prompt_count_args(char *arg_str, size_t len)
{
    char tmp_str[len];
    str_clearcopy(tmp_str, arg_str, sizeof(tmp_str));

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

static void prompt_log_stdin(const char *str)
{
    bool *log_stdin = config_get(CONF_LOG_STDIN);
    if (!*log_stdin)
        return;

    char msg[STDIN_BUFFER_LOG_LEN];
    snprintf(msg, sizeof(msg), "[STDIN]: %s", str);
    log_event(msg);
}

#endif