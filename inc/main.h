#ifndef MAIN_H_DEF
#define MAIN_H_DEF

/*
 File:          main.h
 Description:   Application flow management for the main Peabot application.
 Created:       May 6, 2017
 Author:        Matt Mumau
 */

#define APP_ERROR(msg, error_code) app_error(__FILE__, __LINE__, msg, error_code)

/* Exit the application, citing the given message as a reason. */
void app_exit(int retval);

void app_error(const char *file, int lineno, const char *msg, int error_code);

#endif