#ifndef LOG_DEF
#define LOG_DEF

/*
 File:          log.c
 Description:   Implementation of a file logging mechanism.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

/* System includes */ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "log.h"

FILE *logfile = NULL;

void log_init(char *file)
{
    logfile = fopen(file, "w");

    if (logfile == NULL)
    {
        printf("[LOG LIBRARY EXCEPTION] Could not open logfile!");
        exit(0);
    }
}

void log_write(char *line)
{
    if (logfile == NULL)
        return;

    //fwrite(line, 1, LOG_LINE_MAXLEN, logfile);
    fprintf(logfile, "%s\n", line);
}

void log_close()
{
    if (logfile == NULL)
        return;
    fclose(logfile);
}

void log_h(char *val)
{
    char line_break[80];
    for (int i = 0; i < 80 - 1; i++)
    {
        line_break[i] = '=';
    }
    line_break[80 - 1] = '\0';

    log_write(line_break);
    log_write(val);
    log_write(line_break);
}

void log_br()
{
    char *line_break = calloc(80, sizeof(char));
    for (int i = 0; i < 80; i++)
    {
        line_break[i] = '-';
    }

    log_write(line_break);

    free(line_break);
}

void log_event(char *data)
{
    time_t current_time = time(NULL);
    struct tm *ltime = localtime(&current_time);

    char timestamp[LOG_TIMESTAMP_MAXLEN];
    strftime(timestamp, LOG_TIMESTAMP_MAXLEN, "[%m.%d.%y.%H.%M.%S]", ltime);

    char buffer[LOG_LINE_MAXLEN];
    snprintf(buffer, LOG_LINE_MAXLEN, "%s %s", timestamp, data);

    log_write(buffer);
}

#endif