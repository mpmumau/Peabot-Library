#ifndef UTILS_DEF
#define UTILS_DEF

/*
 File:          utils.c
 Description:   Implementation of a variety of system utility functions.
 Created:       May 6, 2017
 Author:        Matt Mumau
 */

#define _POSIX_C_SOURCE 199309L

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

void utils_mkresponsetime(char *string, size_t len)
{
    time_t time = time(NULL);
    struct tm *ltime;
    ltime = localtime(&time);
    strftime(string, len, "%a, %b %Y %H:%M:%S %Z", ltime);
}

double utils_timediff(struct timespec end_time, struct timespec start_time)
{
    double diff = ((double) (end_time.tv_sec - start_time.tv_sec)) +
        ((double) (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0);
    return diff;
}

double utils_timespec_to_secs(struct timespec ts)
{
    return ((double) ts.tv_sec) + ((double) ((double) ts.tv_nsec / 1000000000.0));
}

#endif