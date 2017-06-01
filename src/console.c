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

/* Application includes */
#include "prompt.h"
#include "string_utils.h"

/* Header */
#include "console.h"

void console_h(const char *msg)
{
    char lb[80];
    str_fill(lb, '=', sizeof(lb));
    printf("%s\n%s\n%s\n", lb, msg, lb);
}

void console_print(const char *msg)
{
    char msg_cpy[CONSOLE_LINE_LEN];
    str_clearcopy(msg_cpy, msg, sizeof(msg_cpy));
    printf("%s\n", msg_cpy);
}

void console_br()
{
    char line[CONSOLE_LINE_LEN];
    for (int i = 0; i < CONSOLE_LINE_LEN - 1; i++)
    {
        line[i] = '-';
    }
    line[CONSOLE_LINE_LEN - 1] = '\0';

    printf("%s\n", line);
}

void console_event(char *data)
{
    time_t current_time = time(NULL);
    struct tm *ltime = localtime(&current_time);

    char *timestamp = calloc(TIMESTAMP_MAXLEN, sizeof(char));
    strftime(timestamp, TIMESTAMP_MAXLEN, "[%m.%d.%y.%H.%M.%S]", ltime);

    char *buffer = calloc(CONSOLE_LINE_LEN, sizeof(char));
    snprintf(buffer, CONSOLE_LINE_LEN, "\n%s %s\n", timestamp, data);

    console_print(buffer);

    free(timestamp);
    free(buffer);    
}

#endif