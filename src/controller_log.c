#ifndef CONTROLLER_LOG_DEF
#define CONTROLLER_LOG_DEF

/*
 File:          controller_log.c
 Description:   Implementation of log controller logic.
 Created:       June 7, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdio.h>
#include <stdlib.h>

/* Application includes */
#include "log.h"
#include "mvc_data.h"

/* Header */
#include "controller_log.h"

bool cntllog_getall(MVCData *mvc_data)
{
    char log_lines[LOG_CACHE_SIZE][LOG_LINE_LEN];
    int len = log_get_cache(log_lines, LOG_CACHE_SIZE, LOG_LINE_LEN);

    for (int i = 0; i < len; i++)
    {
        printf("log line %d: %s\n", i, &log_lines[i][0]);
    }

    return true;
}

#endif