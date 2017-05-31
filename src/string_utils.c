#ifndef STRING_UTILS_DEF
#define STRING_UTILS_DEF

/*
 File:          string_utils.c
 Description:   Various utility functions defs for dealing with strings.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "string_utils.h"

bool str_starts(const char *haystack, const char *needle)
{
    return (bool) (strncmp(haystack, needle, strlen(needle)) == 0);
}

bool str_equals(const char *str_a, const char *str_b)
{
    return (bool) (strcmp(str_a, str_b) == 0);
}

void str_removenl(const char *str)
{
    char *nl = strchr(str, '\n');
    if (nl != NULL)
        *nl = '\0';
}

char *str_tabval(char *str, size_t len)
{
    if (!str || !len)
        return NULL;

    unsigned int i;
    for (i = 0; i < len; i++)
    {
        if (isspace(str[i]))
            break;
    }

    if (!i)
        return NULL;

    for ( ; i < len; i++)
    {
        if (isspace(str[i]))
            continue;
        return &str[i];
    }

    return NULL;
}

bool str_empty(char *str)
{
    if (!str || str[0] == '\0')
        return true;
    return false;
}

void str_clearcopy(char *dest, const char *src, size_t len)
{
    memset(dest, '\0', len);
    memcpy(dest, src, len - 1);
}

void str_linebreak(char *str, size_t len)
{
    if (len < 1)
        return;

    memset(str, '-', len - 1);
    str[len] = '\0';    
}

#endif