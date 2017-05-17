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
#include "robot.h"

/* Header */
#include "config_callbacks.h"

void configset_log_file_dir(Config *config, void *data)
{
    config->log_file_dir = (char *) data;
    return;
}

void configset_log_filename(Config *config, void *data)
{
    config->log_filename = (char *) data;
    return;
}

void configset_log_stdin(Config *config, void *data)
{
    bool *data_p = (bool *) data;
    config->log_stdin = *data_p;
    return;
}

void configset_log_prompt_commands(Config *config, void *data)
{
    bool *data_p = (bool *) data;
    config->log_prompt_commands = *data_p;
}

void configset_log_event_add(Config *config, void *data)
{
    bool *data_p = (bool *) data;
    config->log_event_add = (bool) *data_p;
    return;
}

void configset_log_event_callbacks(Config *config, void *data)
{
    bool *data_p = (bool *) data;
    config->log_event_callbacks = *data_p;
    return;
}

void configset_log_keyframes(Config *config, void *data)
{
    bool *data_p = (bool *) data;
    config->log_keyframes = *data_p;
    return;
}

void configset_pca_9685_pin_base(Config *config, void *data) 
{
    int *data_p = (int *) data;
    config->pca_9685_pin_base = *data_p;
    return;
}

void configset_pca_9685_max_pwm(Config *config, void *data)
{
    int *data_p = (int *) data;
    config->pca_9685_max_pwm = *data_p;
    return;
}

void configset_pca_9685_hertz(Config *config, void *data)
{
    int *data_p = (int *) data;
    config->pca_9685_hertz = *data_p;
    return;
}

void configset_servos_num(Config *config, void *data)
{
    int *data_p = (int *) data;
    config->servos_num = *data_p;
    return;
}

void configset_robot_tick(Config *config, void *data)
{
    float *data_p = (float *) data;
    config->robot_tick = *data_p;
    return;
}

void configset_transitions_enable(Config *config, void *data)
{
    bool *data_p = (bool *) data;
    config->transitions_enable = *data_p;
    return;
}

void configset_transition_time(Config *config, void *data)
{
    float *data_p = (float *) data;
    config->transition_time = *data_p;
    return;
}

void configset_walk_hip_delta(Config *config, void *data)
{
    float *data_p = (float *) data;
    config->walk_hip_delta = *data_p;
    return;
}

void configset_walk_knee_delta(Config *config, void *data)
{
    float *data_p = (float *) data;
    config->walk_knee_delta = *data_p;
    return;
}

void configset_walk_knee_pad_a(Config *config, void *data)
{
    float *data_p = (float *) data;
    config->walk_knee_pad_a = *data_p;
    return;
}

void configset_walk_knee_pad_b(Config *config, void *data)
{
    float *data_p = (float *) data;
    config->walk_knee_pad_b = *data_p;
    return;
}

void configset_servo_pins(Config *config, void *data)
{
    ServoPinData *data_p = (ServoPinData *) data;
    config->servo_pins[data_p->id] = data_p->val;
    return;
}

void configset_servo_limits(Config *config, void *data)
{
    ServoLimitData *data_p = (ServoLimitData *) data;
    ServoLimit *servo_limit = &(config->servo_limits[data_p->id]);
    servo_limit->min = data_p->min;
    servo_limit->max = data_p->max;
    return;
}

#endif