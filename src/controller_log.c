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
    LogLine log_lines[LOG_LINES_BATCH_LEN];

    int lines_num = log_getlines(3, log_lines, sizeof(log_lines));

    printf("Printing log lines...\n");
    printf("Lines num : %d\n", lines_num);

    for (int i = 0; i < lines_num; i++)
    {
        printf("[%d] %s\n", i, log_lines[i]);
    } 

    return true;
}

#endif