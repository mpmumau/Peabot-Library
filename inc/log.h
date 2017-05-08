#ifndef LOG_H_DEF
#define LOG_H_DEF

/*
 File:          log.h
 Description:   Utility functions used for dealing with a log file.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

#ifndef LOG_LINE_MAXLEN
#define LOG_LINE_MAXLEN 128
#endif

#ifndef LOG_TIMESTAMP_MAXLEN
#define LOG_TIMESTAMP_MAXLEN 20
#endif

#ifndef LOG_FILENAME_MAXLEN
#define LOG_FILENAME_MAXLEN 64
#endif

/* Open the given log file for appending. */
void log_init(char *file);

/* Write the given line to the log file. */
void log_write(char *line);

/* Close the log file. */
void log_close();

/* Write a header line in the log file, surrounded by double line breaks. */
void log_h(char *val);

/* Write a single line break to the log. */
void log_br();

/* Write the given data to the log, which will be prepended by a timestamp. */
void log_event(char *data);

#endif