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

/*
 * Copies a properly formatted time string into the provided string.
 */
void utils_mktime(time_t time, char *string);

float utils_nano_to_milli(long nsecs);

#endif