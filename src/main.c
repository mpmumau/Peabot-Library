#ifndef MAIN_DEF
#define MAIN_DEF

/*
 File:          main.c
 Description:   Main application source for the Peabo application.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

/* System dependencies */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

/* Application includes */
#include "config.h"
#include "log.h"
#include "console.h"
#include "prompt.h"
#include "events.h"
#include "keyframe_handler.h"
#include "robot.h"

/* Header */
#include "main.h"

static bool app_running = true;
static int exit_val = 0;

/* Forward decs */
static void signal_handler(int signum);

void app_exit(char *message, int retval)
{
    printf("\n");
    console_br();
    console_print(message);

    log_event(message);

    prompt_halt();
    log_event("[APP_EXIT] Prompt shutdown complete.");

    event_halt();
    log_event("[APP_EXIT] Event handler shutdown complete.");

    keyhandler_halt();
    log_event("[APP_EXIT] Keyframe handler shutdown complete.");

    robot_halt();

    log_event("[APP_EXIT] Robot shutdown complete.");

    config_destroy();

    log_event("[APP_EXIT] Exit sequence complete. Bye!");
    log_close();

    exit_val = retval;
    app_running = false;  
}

/*
 Handles all posix signals.
 */
static void signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        prompt_halt();
        event_halt();
        keyhandler_halt();
        robot_halt();
        
        log_event("POSIX SIGNIT received. Exiting...");
        log_close();

        exit(0);
    }
}

/* Application main */
int main(int argc, char *argv[])
{
    config_init(arc, argv);

    log_init();
    log_h("Peabot Server Logs");
    log_event("Server started.");

    signal(SIGINT, signal_handler);

    console_h("Peabot Server Console"); 
    console_event("Server started");

    robot_init();

    keyhandler_init();

    event_init();
    
    prompt_init();

    while (app_running) {}

    return exit_val;
}

#endif