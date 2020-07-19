/*
 File:          main.c
 Description:   Main object and entrypoint for the Peabot application.
 */

#ifndef MAIN_DEF
#define MAIN_DEF

//#define PEABOT_DBG

/* System dependencies */
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

static unsigned short exit_val = 1;

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

/* Application main */
int main(int argc, char *argv[])
{
    wiringPiSetup();

    robot_init();
    usd_sensor_init();

    exit(exit_val);
    return exit_val;
}

#endif