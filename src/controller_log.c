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

/* Libraries */
#include <cJSON.h>

/* Application includes */
#include "log.h"
#include "mvc_data.h"

/* Header */
#include "controller_log.h"

bool cntllog_getall(MVCData *mvc_data)
{
    if (!mvc_data->response_json)
        return false;

    cJSON *lines_array = cJSON_CreateArray();
    if (!lines_array)
        return false;

    cJSON *tmp_line;

    char log_lines[LOG_CACHE_SIZE][LOG_LINE_LEN];
    int len = log_get_cache(log_lines, 4);

    for (int i = 0; i < len; i++)
    {
        tmp_line = cJSON_CreateString(&log_lines[i][0]);
        if (!tmp_line)
            return false;

        cJSON_AddItemToArray(lines_array, tmp_line);
    }

    cJSON_AddItemToObject(mvc_data->response_json, "log_lines", lines_array);
    return true;
}

#endif