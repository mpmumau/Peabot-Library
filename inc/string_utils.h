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
bool str_starts(const char *haystack, const char *needle);

/* Check if string_a is identical to string_b. */
bool str_equals(const char *str_a, const char *str_b);

/* Remove the first instance of the new line character (\n). */
void str_removenl(const char *str);

/*
 * Return a pointer to the first character after any sequence of spaces in the 
 * string. If the first letter of the string is a space, or there is no tabbed
 * value, return null. 
 */
char *str_tabval(char *str, size_t len);

/* Checks whether a string is NULL or contains no characters. */
bool str_empty(char *str);

/* 
 * Clears all memory in the dest string and copies the src string into it, up to 
 * the size of len.
 */
void str_clearcopy(char *dest, const char *src, size_t len);

/* Returns a hypenated string of size len. */
void str_linebreak(char *str, size_t len);

#endif