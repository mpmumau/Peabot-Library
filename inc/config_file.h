/*
 File:          config_file.h
 Description:   Object for controlling the config file.
 */

#ifndef CONFIG_FILE_H_DEF
#define CONFIG_FILE_H_DEF

/* Load the configuration file and set configuration variables from it. */
void configfile_process(char *config_file_fullpath);

#endif