#ifndef STRING_UTILS_H_DEF
#define STRING_UTILS_H_DEF

/*
 File:          string_utils.h
 Description:   A set of utility functions for dealing with strings.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

#include <stdbool.h>

/* Check if string_a begins with an indentical set of characters to string_b. */
bool str_starts(char * string_a, char * string_b);

/* Check if string_a is identical to string_b. */
bool str_equals(char * string_a, char * string_b);

/* Remove the first instance of the new line character (\n). */
void str_removenl(char *string);

void str_copy(char *dest, char *srv);

char *str_after_spaces(char *string, int limit);

bool str_empty(char *string);

void str_clearcopy(char *dest, char *src, size_t len);

#endif