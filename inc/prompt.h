/*
 File:          prompt.h
 Description:   Functions for dealing with input from the stdin prompt.
 */

#ifndef PROMPT_H_DEF
#define PROMPT_H_DEF

#define STDIN_BUFFER_LEN 256
#define STDIN_BUFFER_LOG_LEN 128

/* Initialize the command prompt thread and beginning taking input. */
void prompt_init();

/* End the command prompt thread. */
void prompt_halt();

/* Print the prompt to standard out. */
void prompt_print();

#endif