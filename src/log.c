#ifndef LOG_DEF
#define LOG_DEF

/*
 File:          log.c
 Description:   Implementation of file logging.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

/* System includes */ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* Application includes */
#include "config_defaults.h"
#include "main.h"
#include "config.h"

/* Header */
#include "log.h"

static FILE *logfile = NULL;

void log_init()
{
    char *filename = (char *) config_get(CONF_LOG_FULLPATH);
    if (!filename)
        app_exit("[ERROR!] Could not open logfile!", 1);

    logfile = fopen(filename, "w");

    if (!logfile)
        app_exit("[ERROR!] Could not open logfile!", 1);
}

void log_write(char *line)
{
    if (!logfile || !line)
        return;
    fprintf(logfile, "%s\n", line);
    fflush (logfile);
}

void log_close()
{
    if (!logfile)
        return;
    fclose(logfile);
}

void log_h(char *val)
{
    char line_break[81];
    for (int i = 0; i < 80 - 1; i++)
        line_break[i] = '=';
    line_break[80] = '\0';

    log_write(line_break);
    log_write(val);
    log_write(line_break);
}

void log_br()
{
    char line_break[81];
    for (int i = 0; i < 80; i++)
        line_break[i] = '-';
    line_break[80] = '\0';

    log_write(line_break);
}

void log_event(char *data)
{
    time_t current_time = time(NULL);
    struct tm *ltime = localtime(&current_time);

    char timestamp[LOG_TIMESTAMP_MAXLEN];
    strftime(timestamp, LOG_TIMESTAMP_MAXLEN, "[%m-%d-%y %H:%M:%S]", ltime);

    char buffer[LOG_LINE_MAXLEN];
    snprintf(buffer, LOG_LINE_MAXLEN, "%s %s", timestamp, data);

    log_write(buffer);
}

#endif