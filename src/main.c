/*
 File:          main.c
 Description:   Main object and entrypoint for the Peabot application.
 */

#ifndef MAIN_DEF
#define MAIN_DEF

//#define PEABOT_DBG

/* System dependencies */
#include <sys/prctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

/* Raspberry Pi includes */
#include <wiringPi.h>

/* Application includes */
#include "robot.h"
#include "usd_sensor.h"

/* Header */
#include "main.h"

static bool running = true;
static unsigned short exit_val = 1;

/* Forward decs */
static void signal_handler(int signum);

void app_exit(int retval)
{
    robot_halt();
    usd_sensor_halt();

    exit_val = retval;
    running = false;
}

void app_error(const char *file, unsigned int lineno, const char *msg, unsigned short error_code)
{
    char err_msg[256];
    snprintf(err_msg, sizeof(err_msg), "[ERROR!] %s [f:%s,l:%d,e:%d]", msg, file, lineno, error_code);
    app_exit(error_code);
}

/*
 Handles all posix signals.
 */
static void signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        app_exit(0);
    }
}

/* Application main */
int main(int argc, char *argv[])
{
    prctl(PR_SET_NAME, "PEABOT_MAIN\0", NULL, NULL, NULL);
    signal(SIGINT, signal_handler);
    wiringPiSetup();

    usd_sensor_init();
    robot_init();

    while (running)
        sleep(1);

    exit(exit_val);
    return exit_val;
}

#endif