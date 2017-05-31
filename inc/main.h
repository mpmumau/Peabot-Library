#ifndef MAIN_H_DEF
#define MAIN_H_DEF

/*
 File:          main.h
 Description:   Application flow management for the main Peabot application.
 Created:       May 6, 2017
 Author:        Matt Mumau
 */

#define APP_VERSION "v0.1 alpha"

#define APP_ERROR(msg, error_code) app_error(__FILE__, __LINE__, msg, error_code)

/* Exit the application, with the given exit code. */
void app_exit(int retval);

/* Exit the application and log an error. */
void app_error(const char *file, unsigned int lineno, const char *msg, unsigned short error_code);

#endif