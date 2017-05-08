#ifndef UTILS_DEF
#define UTILS_DEF

/*
 File:          utils.c
 Description:   Implementation of a variety of system utility functions.
 Created:       May 6, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdio.h>
#include <time.h>
#include <stdio.h>

/* Header */
#include "utils.h"

void utils_mktime(time_t time, char *string)
{
    struct tm *ltime;
    ltime = localtime(&time);

    strftime(string, UTILS_DATETIME_MAXLEN, "%b %d, %Y %H:%M:%S", ltime);
}

float utils_nano_to_milli(long nsecs)
{
    return (float) ((float) nsecs / 100000.0f);
}

#endif