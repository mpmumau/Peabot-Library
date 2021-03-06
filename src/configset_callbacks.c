#ifndef CONFIGSET_CALLBACKS_DEF
#define CONFIGSET_CALLBACKS_DEF

/*
 File:          config_callbacks.c
 Description:   Implementation of configuration variable set functions.
 Created:       May 15, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdbool.h>
#include <stdlib.h>

/* Application includes */
#include "config.h"
#include "robot.h"
#include "string_utils.h"

/* Header */
#include "configset_callbacks.h"

void configset_log_file_dir(Config *config, void *data, bool is_string)
{
    config->log_file_dir = (const char *) data;
    return;
}

void configset_log_filename(Config *config, void *data, bool is_string)
{
    config->log_filename = (const char *) data;
    return;
}

void configset_log_configfile(Config *config, void *data, bool is_string)
{
    config->config_file = (const char *) data;
    return;
}

void configset_log_stdin(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->log_stdin = str_equals((const char *) data, "true") ? true : false;
    else
    {
        bool *data_p = (bool *) data;
        config->log_stdin = *data_p;
    }
    
    return;
}

void configset_log_prompt_commands(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->log_prompt_commands = str_equals((const char *) data, "true") ? true : false;
    else
    {
        bool *data_p = (bool *) data;
        config->log_prompt_commands = *data_p;
    }

    return;
}

void configset_log_event_add(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->log_event_add = str_equals((const char *) data, "true") ? true : false;
    else
    {
        bool *data_p = (bool *) data;
        config->log_event_add = *data_p;
    }

    return;
}

void configset_log_event_callbacks(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->log_event_callbacks = str_equals((const char *) data, "true") ? true : false;
    else
    {
        bool *data_p = (bool *) data;
        config->log_event_callbacks = *data_p;
    }

    return;
}

void configset_log_keyframes(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->log_keyframes = str_equals((const char *) data, "true") ? true : false;
    else
    {
        bool *data_p = (bool *) data;
        config->log_keyframes = *data_p;
    }

    return;
}

void configset_pca_9685_pin_base(Config *config, void *data, bool is_string) 
{
    if (is_string)
        config->pca_9685_pin_base = (unsigned int) atoi((const char *) data);
    else
    {
        unsigned int *data_p = (unsigned int *) data;
        config->pca_9685_pin_base = *data_p;
    }

    return;
}

void configset_pca_9685_max_pwm(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->pca_9685_max_pwm = (unsigned int) atoi((const char *) data);
    else
    {
        unsigned int *data_p = (unsigned int *) data;
        config->pca_9685_max_pwm = *data_p;
    }

    return;
}

void configset_pca_9685_hertz(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->pca_9685_hertz = (unsigned int) atoi((const char *) data);
    else
    {
        unsigned int *data_p = (unsigned int *) data;
        config->pca_9685_hertz = *data_p;
    }

    return;
}

void configset_servos_num(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->servos_num = (unsigned short) atoi((const char *) data);
    else
    {
        unsigned short *data_p = (unsigned short *) data;
        config->servos_num = *data_p;
    }

    return;
}

void configset_robot_tick(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->robot_tick = (double) atof((const char *) data);
    else
    {
        double *data_p = (double *) data;
        config->robot_tick = *data_p;
    }

    return;
}

void configset_transitions_enable(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->transitions_enable = str_equals((const char *) data, "true") ? true : false;
    else
    {
        bool *data_p = (bool *) data;
        config->transitions_enable = *data_p;
    }

    return;
}

void configset_transition_time(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->transition_time = (double) atof((const char *) data);
    else
    {
        double *data_p = (double *) data;
        config->transition_time = *data_p;
    }

    return;
}

void configset_walk_hip_delta(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->walk_hip_delta = (double) atof((const char *) data);
    else
    {
        double *data_p = (double *) data;
        config->walk_hip_delta = *data_p;
    }

    return;
}

void configset_walk_knee_delta(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->walk_knee_delta = (double) atof((const char *) data);
    else
    {
        double *data_p = (double *) data;
        config->walk_knee_delta = *data_p;
    }

    return;
}

void configset_walk_knee_pad_a(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->walk_knee_pad_a = (double) atof((const char *) data);
    else
    {
        double *data_p = (double *) data;
        config->walk_knee_pad_a = *data_p;
    }

    return;
}

void configset_walk_knee_pad_b(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->walk_knee_pad_b = (double) atof((const char *) data);
    else
    {
        double *data_p = (double *) data;
        config->walk_knee_pad_b = *data_p;
    }

    return;
}

void configset_servo_pins(Config *config, void *data, bool is_string)
{
    ServoPinData *data_p = (ServoPinData *) data;
    config->servo_pins[data_p->id] = data_p->val;
    return;
}

void configset_servo_limits(Config *config, void *data, bool is_string)
{
    ServoLimitData *data_p = (ServoLimitData *) data;
    ServoLimit *servo_limit = &(config->servo_limits[data_p->id]);
    servo_limit->min = data_p->min;
    servo_limit->max = data_p->max;
    return;
}

void configset_http_enabled(Config *config, void *data, bool is_string)
{
    if (is_string)
         config->http_enabled = str_equals((const char *) data, "true") ? true : false;
    else
    {
        bool *data_p = (bool *) data;
        config->http_enabled = *data_p;
    }

    return;
}

void configset_http_port(Config *config, void *data, bool is_string)
{
    if (is_string)
        config->http_port = (unsigned short) atof((const char *) data);
    else
    {
        int *data_p = (int *) data;
        config->http_port = *data_p;
    }

    return;
}

#endif