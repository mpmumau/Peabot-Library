#ifndef CONSOLE_H_DEF
#define CONSOLE_H_DEF

/*
 File:          console.h
 Description:   A variety of utility functions for printing to standard output;
                In other words the console.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

#define CONSOLE_LINE_LEN 128
#define TIMESTAMP_MAXLEN 20

/* Print a header message to the console; will be surrounded by two double lines. */
void console_h(const char *msg);

/* Print the given string to a line on the console. */
void console_print(const char *msg);

/* Print a single line break. */
void console_br();

/* Print a message to the console, prepended by a timestamp */
void console_event(char *data);

#endif