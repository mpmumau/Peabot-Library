/*
 File:          console.h
 Description:   A variety of utility functions for printing to standard output;
                In other words the console.
 */

#ifndef CONSOLE_H_DEF
#define CONSOLE_H_DEF

#define CONSOLE_LINE_LEN 128
#define TIMESTAMP_MAXLEN 20

/* Print a header message to the console; will be surrounded by two double lines. */
void console_h(const char *msg);

/* Print the given string to a line on the console. */
void console_print(const char *msg);

/* Print an error message to the console. */
void console_error(const char *msg);

#endif