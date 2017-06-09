#ifndef CONTROLLER_LOG_H_DEF
#define CONTROLLER_LOG_H_DEF

/*
 File:          controller_log.h
 Description:   Declarations of functions for the log controller.
 Created:       June 7, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdbool.h>

#define LOG_LINES_BATCH_LEN 256

/* Handle a GET request to retrieve all log items. */
bool cntllog_getall(MVCData *mvc_data);

#endif