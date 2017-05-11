#ifndef PROMPT_H_DEF
#define PROMPT_H_DEF

/*
 File:          prompt.h
 Description:   Functions for dealing with input from a standard in prompt.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

/* Initialize the command prompt thread and beginning taking input. */
void prompt_init();

/* End the command prompt thread. */
void prompt_halt();

#endif