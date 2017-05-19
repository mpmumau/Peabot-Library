#ifndef CONFIG_STDIN_H_DEF
#define CONFIG_STDIN_H_DEF

/*
 File:          config_stdin.h
 Description:   Managing object for all piped-in (stdin) configuration values.
 Created:       May 19, 2017
 Author:        Matt Mumau
 */

/* Send data to be handled from the command line arguments to the stdin pipeline. */
void configstdin_pipe(int argc, char *argv[]);

#endif 