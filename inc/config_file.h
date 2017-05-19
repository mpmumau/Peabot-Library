#ifndef CONFIG_FILE_H_DEF
#define CONFIG_FILE_H_DEF

/*
 File:          config_file.h
 Description:   Object for controlling the config file.
 Created:       May 19, 2017
 Author:        Matt Mumau
 */

#define COMMENT_CHARACTER "#"
#define CONFIG_DELIM " "

/* Load the configuration file and set configuration variables from it. */
void configfile_process(char *config_file_fullpath);

#endif