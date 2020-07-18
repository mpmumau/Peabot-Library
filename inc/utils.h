/*
 File:          utils.h
 Description:   Various system-level utility functions.
 */

#ifndef UTILS_H
#define UTILS_H

#ifndef UTILS_DATETIME_MAXLEN
#define UTILS_DATETIME_MAXLEN 23
#endif

/* Copies a properly formatted time string into the provided string. */
void utils_mktime(time_t time, char *string);

/* Get a time format appropriate for an HTTP response */
void utils_mkresponsetime(char *string, size_t len);

/* Get the difference in seconds between the timespecs; nano-second precision) */
double utils_timediff(struct timespec end_time, struct timespec start_time);

double utils_timespec_to_secs(struct timespec ts);

#endif