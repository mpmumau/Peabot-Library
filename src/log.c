#ifndef LOG_DEF
#define LOG_DEF

/*
 File:          log.c
 Description:   Implementation of file logging.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

/* System includes */ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/* Application includes */
#include "config_defaults.h"
#include "main.h"
#include "config.h"
#include "string_utils.h"

/* Header */
#include "log.h"

/* Forward decs */
static void log_cache_line(char *line);

static FILE *logfile = NULL;
bool is_active;

static char *log_cache[LOG_CACHE_SIZE];
static short log_cache_index = 0;

void log_init()
{
    char *filename = (char *) config_get(CONF_LOG_FULLPATH);
    if (!filename)
        APP_ERROR("Log file name is null.", 1);

    logfile = fopen(filename, "w");
    if (!logfile)
        APP_ERROR("Could not open log file.", 1);

    log_cache_index = 0;

    char hdr_line[LOG_LINE_LEN];
    snprintf(hdr_line, sizeof(hdr_line), "Peabot Server: %s", APP_VERSION);

    log_h(hdr_line);
}

void log_write(char *line)
{
    if (!logfile || !line)
        return;

    while (is_active) {} // block while being read from

    is_active = true;

    char line_cpy[LOG_LINE_LEN];
    str_clearcopy(line_cpy, line, sizeof(line_cpy));

    log_cache_line(line);

    fprintf(logfile, "%s\n", line_cpy);
    fflush (logfile);

    is_active = false;
}

void log_close()
{
    if (!logfile)
        return;
    fclose(logfile);
}

void log_h(char *val)
{
    char lb[128];
    str_fill(lb, '-', sizeof(lb));
    
    log_write(lb);
    log_write(val);
    log_write(lb);
}

void log_br()
{
    char line_break[81];
    for (int i = 0; i < 80; i++)
        line_break[i] = '-';
    line_break[80] = '\0';

    log_write(line_break);
}

void log_event(const char *msg)
{
    time_t current_time = time(NULL);
    struct tm *ltime = localtime(&current_time);

    char timestamp[LOG_TIMESTAMP_MAXLEN];
    strftime(timestamp, LOG_TIMESTAMP_MAXLEN, "[%m-%d-%y %H:%M:%S]", ltime);

    char buffer[LOG_LINE_MAXLEN];
    snprintf(buffer, sizeof(buffer), "%s %s", timestamp, msg);

    log_write(buffer);
}

void log_error(const char *msg, int error_code)
{
    char log_msg[LOG_LINE_LEN];
    snprintf(log_msg, sizeof(log_msg), "[ERROR] %s [e:%d]", msg, error_code);

    log_event(log_msg);
}

static void log_cache_line(char *line)
{
    char *tmp = log_cache[log_cache_index];
    str_clearcopy(tmp, line, LOG_LINE_LEN);
    log_cache_index++;

    printf("last log line: %s\n", tmp);

    if (log_cache_index >= sizeof(log_cache))
        log_cache_index = 0;

    printf("current log cache...\n");
    for (int i = 0; i < log_cache_index; i++) {
        printf("[LC STORED %d] %s", i, log_cache[i]);
    }
}

// int log_get_cache(char *lines[], size_t len, size_t item_len) 
// {
//     for (int i = 0; i < len; i++)
//     {
//         if (i >= log_cache_index)
//             break;

//         printf("[LC Line %d] %s\n", i, log_cache[i]);
//     }
// }

#endif