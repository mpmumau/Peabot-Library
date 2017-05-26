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

void utils_mkresponsetime(time_t time, char *string)
{
    struct tm *ltime;
    ltime = localtime(&time);

    strftime(string, UTILS_DATETIME_MAXLEN, "%a, %b %Y %H:%M:%S %Z", ltime);
}

double utils_timediff(struct timespec end_time, struct timespec start_time)
{
    double diff = ((double) (end_time.tv_sec - start_time.tv_sec)) +
        ((double) (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0);
    return diff;
}

#endif