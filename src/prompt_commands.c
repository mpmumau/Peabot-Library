/*
 File:          prompt_commands.c
 Description:   Implementation of prompt command callback functions.
 */

#ifndef PROMPT_COMMANDS_DEF
#define PROMPT_COMMANDS_DEF

/* System includes */ 
#include <stdlib.h>
#include <stdio.h>

/* Application includes */
#include "config_defaults.h"
#include "main.h"
#include "config.h"
#include "log.h"
#include "console.h"
#include "string_utils.h"

/* Header */
#include "prompt_commands.h"

static void promptcmd_log_cmd(const char *msg);
static bool promptcmd_check_args(const char *usage_str, unsigned short args_req, unsigned short args_num);

static void promptcmd_log_cmd(const char *msg)
{
    bool *log_prompt_commands = (bool *) config_get(CONF_LOG_PROMPT_COMMANDS);
    if (!*log_prompt_commands)
        return;

    char log_msg[LOG_LINE_MAXLEN];
    snprintf(log_msg, LOG_LINE_MAXLEN, "[PRMT] %s", msg);
    log_event(log_msg);
}

static bool promptcmd_check_args(const char *usage_str, unsigned short args_req, unsigned short args_num)
{
    if (args_req != args_num)
    {
        char cns_msg[CONSOLE_LINE_LEN];
        snprintf(cns_msg, sizeof(cns_msg), "Incorrect number of params. Usage: %s", usage_str);
        console_error(cns_msg);
        return false;
    }

    return true;
}

void promptcmd_quit(char *args[], int arg_num)
{
    app_exit(0);
}

void promptcmd_cfg_get(char *args[], int arg_num)
{
    const char *var_name = args[0];

    if (str_equals(var_name, "log_file_dir"))
    {
        const char *log_file_dir = (const char *) config_get(CONF_LOG_FILE_DIR);
        printf("[Config] log_file_dir: %s\n", log_file_dir);
        return;
    }

    if (str_equals(var_name, "log_filename"))
    {
        const char *log_filename = (const char *) config_get(CONF_LOG_FILENAME);
        printf("[Config] log_filename: %s\n", log_filename);
        return;
    }

    if (str_equals(var_name, "log_fullpath"))
    {
        const char *log_fullpath = (const char *) config_get(CONF_LOG_FULLPATH);
        printf("[Config] log_fullpath: %s\n", log_fullpath);
        return;
    }

    if (str_equals(var_name, "config_file"))
    {
        const char *config_file = (const char *) config_get(CONF_CONFIG_FILE);
        printf("[Config] config_file: %s\n", config_file);
        return;
    }

    if (str_equals(var_name, "log_stdin"))
    {
        bool *val = (bool *) config_get(CONF_LOG_STDIN);
        printf("[Config] log_stdin: %s\n", *val ? "true" : "false");
        return;
    }

    if (str_equals(var_name, "log_prompt_commands"))
    {
        bool *val = (bool *) config_get(CONF_LOG_PROMPT_COMMANDS);
        printf("[Config] log_prompt_commands: %s\n", *val ? "true" : "false");
        return;
    }

    if (str_equals(var_name, "log_event_add"))
    {
        bool *val = (bool *) config_get(CONF_LOG_EVENT_ADD);
        printf("[Config] log_event_add: %s\n", *val ? "true" : "false");
        return;
    }

    if (str_equals(var_name, "log_event_callbacks"))
    {
        bool *val = (bool *) config_get(CONF_LOG_EVENT_CALLBACKS);
        printf("[Config] log_event_callbacks: %s\n", *val ? "true" : "false");
        return;
    }

    if (str_equals(var_name, "log_keyframes"))
    {
        bool *val = (bool *) config_get(CONF_LOG_KEYFRAMES);
        printf("[Config] log_keyframes: %s\n", *val ? "true" : "false");
        return;
    }

    if (str_equals(var_name, "pca_9685_pin_base"))
    {
        unsigned int *val = (unsigned int *) config_get(CONF_PCA_9685_PIN_BASE);
        printf("[Config] pca_9685_pin_base: %i\n", *val);
        return;
    }

    if (str_equals(var_name, "pca_9685_max_pwm"))
    {
        unsigned int *val = (unsigned int *) config_get(CONF_PCA_9685_MAX_PWM);
        printf("[Config] pca_9685_max_pwm: %i\n", *val);
        return;
    }

    if (str_equals(var_name, "pca_9685_hertz"))
    {
        unsigned int *val = (unsigned int *) config_get(CONF_PCA_9685_HERTZ);
        printf("[Config] pca_9685_hertz: %i\n", *val);
        return;
    }

    if (str_equals(var_name, "servos_num"))
    {
        unsigned short *val = (unsigned short *) config_get(CONF_SERVOS_NUM);
        printf("[Config] servos_num: %i\n", *val);
        return;
    }

    if (str_equals(var_name, "robot_tick"))
    {
        double *val = (double *) config_get(CONF_ROBOT_TICK);
        printf("[Config] robot_tick: %f\n", *val);
        return;
    }

    if (str_equals(var_name, "transitions_enable"))
    {
        bool *val = (bool *) config_get(CONF_TRANSITIONS_ENABLE);
        printf("[Config] transitions_enable: %s\n", *val ? "true" : "false");
        return;
    }

    if (str_equals(var_name, "transitions_time"))
    {
        double *val = (double *) config_get(CONF_TRANSITIONS_TIME);
        printf("[Config] transitions_time: %f\n", *val);
        return;
    }

    if (str_equals(var_name, "walk_hip_delta"))
    {
        double *val = (double *) config_get(CONF_WALK_HIP_DELTA);
        printf("[Config] walk_hip_delta: %f\n", *val);
        return;
    }

    if (str_equals(var_name, "walk_knee_delta"))
    {
        double *val = (double *) config_get(CONF_WALK_KNEE_DELTA);
        printf("[Config] walk_knee_delta: %f\n", *val);
        return;
    }

    if (str_equals(var_name, "walk_knee_pad_a"))
    {
        double *val = (double *) config_get(CONF_WALK_KNEE_PAD_A);
        printf("[Config] walk_knee_pad_a: %f\n", *val);
        return;
    }

    if (str_equals(var_name, "walk_knee_pad_b"))
    {
        double *val = (double *) config_get(CONF_WALK_KNEE_PAD_B);
        printf("[Config] walk_knee_pad_b: %f\n", *val);
        return;
    }

    if (str_equals(var_name, "servo_pin"))
    {
        if (arg_num < 2)
        {
            console_print("[ERROR] Incorrect number of params. Usage: cfg_get servo_pin [index]]");
            return;
        }

        unsigned short index = (unsigned short) atoi(args[1]);
        unsigned short *pins = config_get(CONF_SERVO_PINS);

        unsigned short *servos_num = (unsigned short *) config_get(CONF_SERVOS_NUM);
        if (index > *servos_num - 1)
        {
            console_print("[ERROR] Provided index is greater than the number of servos.");
            return;
        }

        printf("[Config] servo_pin@%d: %d\n", index, pins[index]);
        return ;
    }

    if (str_equals(var_name, "servo_limit"))
    {
        if (arg_num < 2)
        {
            console_print("[ERROR] Incorrect number of params. Usage: cfg_get servo_limit [index]]");
            return;
        }

        unsigned short index = (unsigned short) atoi(args[1]);

        unsigned short *servos_num = (unsigned short *) config_get(CONF_SERVOS_NUM);
        if (index > *servos_num - 1)
        {
            console_print("[ERROR] Provided index is greater than the number of servos.");
            return;
        }

        ServoLimit *servo_limits = (ServoLimit *) config_get(CONF_SERVO_LIMITS);

        printf("[Config] servo_limit@%d: [min] %d [max] %d\n", index, servo_limits[index].min, servo_limits[index].max);
        return;
    }

    console_print("[ERROR] Could not get config variable with specified name.");
    return;
}

#endif