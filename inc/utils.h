#ifndef UTILS_H
#define UTILS_H

/*
 File:          utils.h
 Description:   Various system-level utility functions.
 Created:       May 6, 2017
 Author:        Matt Mumau
 */

#ifndef UTILS_DATETIME_MAXLEN
#define UTILS_DATETIME_MAXLEN 23
#endif

/* Copies a properly formatted time string into the provided string. */
void utils_mktime(time_t time, char *string);

/* Get a time format appropriate for an HTTP response */
void utils_mkresponsetime(time_t time, char *string)

/* Get the difference in seconds between the timespecs; nano-second precision) */
float utils_timediff(struct timespec end_time, struct timespec start_time);

#endif