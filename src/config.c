#ifndef CONFIG_DEF
#define CONFIG_DEF

/*
 File:          config.c
 Description:   Implementation of config functions.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

/* System includes*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Application includes */
#include "log.h"
#include "config.h"
#include "string_utils.h"

/* Forward decs */
void config_log_filename();
void config_handle_arg(char *arg, char *val);

/* Config variables */
static char log_filename[LOG_FILENAME_MAXLEN];

void config_pipe(int argc, char *argv[])
{
    if (argc <= 1)
        return;

    char *buffer;
    char *arg;
    char *val;

    for (int args_counter = 1; args_counter < argc; args_counter++)
    {
        buffer = argv[args_counter];

        if (!str_starts(buffer, "-"))
        {
            continue;
        }

        if (!str_starts(argv[args_counter + 1], "-"))
        {
            val = argv[args_counter + 1];
            args_counter++;
        }

        buffer++;
        arg = buffer;

        config_handle_arg(arg, val);
    }
}

void config_init()
{
    config_log_filename();
}

void config_handle_arg(char *arg, char *val)
{
    if (str_equals(arg, "c")) {
        // handle config file
    }

    if (str_equals(arg, "rt"))
    {
        // handle setting the robot tick speed from the command line
    }    
}

void config_log_filename()
{
    time_t current_time = time(NULL);
    struct tm *ltime = localtime(&current_time);

    snprintf(log_filename, LOG_FILENAME_MAXLEN, 
        "/var/log/peabot/peabot.%d-%d-%d-%d-%d-%d.log", 
        ltime->tm_mon, 
        ltime->tm_mday, 
        ltime->tm_year % 100, 
        ltime->tm_hour, 
        ltime->tm_min, 
        ltime->tm_sec);  
}

char *config_logfile()
{
    return log_filename;
}

#endif