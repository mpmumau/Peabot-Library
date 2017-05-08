#ifndef MAIN_DEF
#define MAIN_DEF

/*
 File:          main.c
 Description:   Main application source for the Peabo application.
 Created:       April 2, 2017
 Author:        Matt Mumau
 */

/* System dependencies */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

/* Header */
#include "main.h"

/* Application includes */
#include "config.h"
#include "log.h"
#include "console.h"
#include "prompt.h"
#include "utils.h"
#include "string_utils.h"
#include "robot.h"

/* Application config */
bool running = true;
int exit_val = 0;

/* Forward decs */
void signal_handler(int signum);
void parse_args(int argc, char * argv[]);
void handle_command(char *stdin);

/*
 Handles all posix signals.
 */
void signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        app_exit("POSIX int signal received. Shutting down.", 0);
    }
}

void app_exit(char *message, int retval)
{
    printf("\n");
    console_br();
    console_print(message);

    log_event(message);

    robot_halt();
    log_event("Robot shutdown.");

    prompt_halt();
    log_event("Prompt shutdown.");

    log_close();

    exit_val = retval;
    running = false;  
}

/* Application main */
int main(int argc, char *argv[])
{
    config_init();
    config_pipe(argc, argv);

    char *log_filename = config_logfile();
    log_init(log_filename);
    log_h("Peabot Server Logs");
    log_event("Server started.");

    signal(SIGINT, signal_handler);

    console_h("Peabot Server Console"); 
    console_event("Server started");

    prompt_init();

    robot_init();

    while (running)
    {
    }

    return exit_val;
}
#endif