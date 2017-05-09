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

/* Header */
#include "utils.h"

void utils_mktime(time_t time, char *string)
{
    struct tm *ltime;
    ltime = localtime(&time);

    strftime(string, UTILS_DATETIME_MAXLEN, "%b %d, %Y %H:%M:%S", ltime);
}

float utils_timediff(struct timespec end_time, struct timespec start_time)
{
    float diff = ((float) end_time.tv_sec - (float) start_time.tv_sec) +
        ((float) end_time.tv_nsec - (float) start_time.tv_nsec) / 1000000000.0f;
    return diff;
}

#endif