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
#include "string_utils.h"

/* Header */
#include "log.h"

static FILE *logfile = NULL;

void log_init()
{
    char *filename = (char *) config_get(CONF_LOG_FULLPATH);
    if (!filename)
        APP_ERROR("Log file name is null.", 1);

    logfile = fopen(filename, "w");
    if (!logfile)
        APP_ERROR("Could not open log file.", 1);
}

void log_write(char *line)
{
    if (!logfile || !line)
        return;

    char line_cpy[LOG_LINE_LEN];
    str_clearcopy(line_cpy, line, sizeof(line_cpy));

    fprintf(logfile, "%s\n", line_cpy);
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
    char *lb[] = str_linebreak(128);
    log_write(lb);
    log_write(val);
    log_write(lb);
}

void log_br()
{
    char line_break[81];
    for (int i = 0; i < 80; i++)
        line_break[i] = '-';
    line_break[80] = '\0';

    log_write(line_break);
}

void log_event(const char *msg)
{
    time_t current_time = time(NULL);
    struct tm *ltime = localtime(&current_time);

    char timestamp[LOG_TIMESTAMP_MAXLEN];
    strftime(timestamp, LOG_TIMESTAMP_MAXLEN, "[%m-%d-%y %H:%M:%S]", ltime);

    char buffer[LOG_LINE_MAXLEN];
    snprintf(buffer, sizeof(buffer), "%s %s", timestamp, msg);

    log_write(buffer);
}

void log_error(const char *msg, int error_code)
{
    char log_msg[LOG_LINE_LEN];
    snprintf(log_msg, sizeof(log_msg), "[ERROR] %s [e:%d]", msg, error_code);

    log_event(log_msg);
}

#endif