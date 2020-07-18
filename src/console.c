/*
 File:          console.c
 Description:   Implementations of console related functions.
 */

#ifndef CONSOLE_DEF
#define CONSOLE_DEF

/* System includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* Application includes */
#include "prompt.h"
#include "string_utils.h"

/* Header */
#include "console.h"

void console_h(const char *msg)
{
    char lb[80];
    str_fill(lb, '=', sizeof(lb));
    printf("%s\n%s\n%s\n", lb, msg, lb);
}

void console_print(const char *msg)
{
    char msg_cpy[CONSOLE_LINE_LEN];
    str_clearcopy(msg_cpy, msg, sizeof(msg_cpy));
    printf("%s\n", msg_cpy);
}

void console_error(const char *msg)
{
    char msg_cpy[CONSOLE_LINE_LEN];
    snprintf(msg_cpy, sizeof(msg_cpy), "[ERROR] %s", msg);
    console_print(msg_cpy);
}

#endif