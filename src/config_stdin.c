#ifndef CONFIG_STDIN_DEF
#define CONFIG_STDIN_DEF

/*
 File:          config_stdin.c
 Description:   Manage standard-in (piped-in) configuration variables.
 Created:       May 19, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdio.h>

/* Application includes */
#include "config.h"
#include "string_utils.h"

/* Header */
#include "config_stdin.h"

/* Forward decs */
static void config_handle_arg(char *arg, char *val);

void configstdin_pipe(int argc, char *argv[])
{
    if (argc <= 1)
        return;

    char *cursor;
    char *next;

    char *arg;
    char *val;

    for (int i = 1; i < argc; i++)
    {
        arg = NULL;
        val = NULL;

        cursor = argv[i];
        next = argv[i + 1];

        if (!str_starts(cursor, "-"))
            continue;

        if (!str_starts(next, "-"))
        {
            val = next;
            i++;
        }

        arg = cursor;

        config_handle_arg(arg, val);
    }
}

static void config_handle_arg(char *arg, char *val)
{
    if (str_equals(arg, "-c") || str_equals(arg, "--config")) 
        config_set(CONF_CONFIG_FILE, (void *) val, true);

    if (str_equals(arg, "--log_stdin"))
        config_set(CONF_LOG_STDIN, (void *) val, true);

    if (str_equals(arg, "--log_prompt"))
        config_set(CONF_LOG_PROMPT_COMMANDS, (void *) val, true);

    if (str_equals(arg, "--log_event_add"))
        config_set(CONF_LOG_EVENT_ADD, (void *) val, true);

    if (str_equals(arg, "--log_event_callback"))
        config_set(CONF_LOG_EVENT_CALLBACKS, (void *) val, true);

    if (str_equals(arg, "--log_keyframes"))
        config_set(CONF_LOG_KEYFRAMES, (void *) val, true);            

    if (str_equals(arg, "--pca-9685-hertz"))
        config_set(CONF_PCA_9685_HERTZ, (void *) val, true);

    if (str_equals(arg, "--pca-9685-pin-base")) 
        config_set(CONF_PCA_9685_PIN_BASE, (void *) val, true);

    if (str_equals(arg, "--pca-9685-max-pwm"))
        config_set(CONF_PCA_9685_MAX_PWM, (void *) val, true);

    if (str_equals(arg, "-s") || str_equals(arg, "--servos"))
        config_set(CONF_SERVOS_NUM, (void *) val, true);

    if (str_equals(arg, "-t") || str_equals(arg, "--servo_tick"))
        config_set(CONF_ROBOT_TICK, (void *) val, true);

    if (str_equals(arg, "--transitions-enable"))
        config_set(CONF_TRANSITIONS_ENABLE, (void *) val, true);

    if (str_equals(arg, "--transitions-time"))
        config_set(CONF_TRANSITIONS_TIME, (void *) val, true);

    if (str_equals(arg, "--walk-hip-delta"))
        config_set(CONF_WALK_HIP_DELTA, (void *) val, true);

    if (str_equals(arg, "--walk-knee-delta"))
        config_set(CONF_WALK_KNEE_DELTA, (void *) val, true);

    if (str_equals(arg, "--walk-knee-pad-a"))
        config_set(CONF_WALK_KNEE_PAD_A, (void *) val, true);    

    if (str_equals(arg, "--walk-knee-pad-b"))
        config_set(CONF_WALK_KNEE_PAD_B, (void *) val, true); 

    if (str_equals(arg, "--http_enabled"))
        config_set(CONF_HTTP_ENABLED, (void *) val, true);

    if (str_equals(arg, "-p") || str_equals(arg, "--port"))
        config_set(CONF_HTTP_PORT, (void *) val, true); 
}
#endif