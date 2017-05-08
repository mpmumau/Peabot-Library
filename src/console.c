#ifndef CONSOLE_DEF
#define CONSOLE_DEF

/*
 File:          console.c
 Description:   Implementations of console related functions.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "console.h"

void console_h(char *val)
{
    char line[CONSOLE_LINE_MAXLEN];
    for (int i = 0; i < CONSOLE_LINE_MAXLEN - 1; i++)
    {
        line[i] = '=';
    }
    line[CONSOLE_LINE_MAXLEN - 1] = '\0';

    console_print(line);
    console_print(val);
    console_print(line);
}

void console_print(char *val)
{
    printf("%s\n", val);
}

void console_br()
{
    char line[CONSOLE_LINE_MAXLEN];
    for (int i = 0; i < CONSOLE_LINE_MAXLEN - 1; i++)
    {
        line[i] = '-';
    }
    line[CONSOLE_LINE_MAXLEN - 1] = '\0';

    printf("%s\n", line);
}

void console_event(char *data)
{
    time_t current_time = time(NULL);
    struct tm *ltime = localtime(&current_time);

    char *timestamp = calloc(TIMESTAMP_MAXLEN, sizeof(char));
    strftime(timestamp, TIMESTAMP_MAXLEN, "[%m.%d.%y.%H.%M.%S]", ltime);

    char *buffer = calloc(CONSOLE_LINE_MAXLEN, sizeof(char));
    snprintf(buffer, CONSOLE_LINE_MAXLEN, "%s %s\n", timestamp, data);

    console_print(buffer);

    free(timestamp);
    free(buffer);    
}

#endif