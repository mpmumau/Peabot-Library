#ifndef CONFIG_CALLBACKS_DEF
#define CONFIG_CALLBACKS_DEF

/*
 File:          config_callbacks.c
 Description:   Implementation of configuration variable set functions.
 Created:       May 15, 2017
 Author:        Matt Mumau
 */

/* Application includes */
#include "config.h"

/* Header */
#include "config_callbacks.h"

void configset_log_file_dir(Config *config, (void *) data)
{
    config->log_file_dir = (char *) data;
    return;
}

void configset_log_filename(Config *config, (void *) data)
{
    config->log_filename = (char *) data;
    return;
}

void configset_log_stdin(Config *config, (void *) data)
{
    config->log_stdin = (bool) *data;
    return;
}

void configset_log_prompt_commands(Config *config, (void *) data)
{
    config->log_prompt_commands = (bool) *data;
}

void configset_log_event_add(Config *config, (void *) data)
{
    config->log_event_add = (bool) *data;
    return;
}

void configset_log_event_callbacks(Config *config, (void *) data)
{
    config->log_event_callbacks = (bool) *data;
    return;
}

void configset_log_keyframes(Config *config, (void *) data)
{
    config->log_keyframes = (bool) *data;
    return;
}

void configset_pca_9685_pin_base(Config *config, (void *) data) 
{
    config->pca_9685_pin_base = (int) *data;
    return;
}

void configset_pca_9685_max_pwm(Config *config, (void *) data)
{
    config->pca_9685_max_pwm = (int) *data;
    return;
}

void configset_pca_9685_hertz(Config *config, (void *) data)
{
    config->pca_9685_hertz = (int) *data;
    return;
}

void configset_servos_num(Config *config, (void *) data)
{
    config->servos_num = (int) *data;
    return;
}

void configset_robot_tick(Config *config, (void *) data)
{
    config->robot_tick = (float) *data;
    return;
}

void configset_transitions_enable(Config *config, (void *) data)
{
    config->transitions_enable = (bool) *data;
    return;
}

void configset_transition_time(Config *config, (void *) data)
{
    config->transition_time = (float) *data;
    return;
}

void configset_walk_hip_delta(Config *config, (void *) data)
{
    config->walk_hip_delta = (float) *data;
    return;
}

void configset_walk_knee_delta(Config *config, (void *) data)
{
    config->walk_knee_delta = (float) *data;
    return;
}

void configset_walk_knee_pad_a(Config *config, (void *) data)
{
    config->walk_knee_pad_a = (float) *data;
    return;
}

void configset_walk_knee_pad_b(Config *config, (void *) data)
{
    config->walk_knee_pad_b = (float) *data;
    return;
}

void configset_servo_pins(Config *config, (void *) data)
{
    ServoPinData *servo_pin_data = (ServoPinData *) data;
    config->servo_pins[servo_pin_data->id] = servo_pin_data->val;
    return;
}

void configset_servo_limits(Config *config, (void *) data)
{
    ServoLimitData servo_limit_data = (ServoLimitData *) data;
    config->servo_pins[servo_pin_data->id].min = servo_pin_data->min;
    config->servo_pins[servo_pin_data->id].max = servo_pin_data->max;
    return;
}

#endif