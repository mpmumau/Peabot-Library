#ifndef CONFIG_CALLBACKS_H_DEF
#define CONFIG_CALLBACKS_H_DEF

/*
 File:          config_callbacks.h
 Description:   Functions for setting configuration variables from abstract data objects.
 Created:       May 15, 2017
 Author:        Matt Mumau
 */

#include "config.h"

/* Set the log file's directory; takes a char pointer, cast to a void pointer. */
void configset_log_file_dir(Config config, (void *) data);
/* Set the log file's filename; takes a char pointer, cast to a void pointer. */
void configset_log_filename(Config config, (void *) data);

/* Set wheter to log prompt stdin; takes a bool pointer, cast to a void pointer. */
void configset_log_stdin(Config config, (void *) data);
/* Set wheter to log prompt commands; takes a bool pointer, cast to a void pointer. */
void configset_log_prompt_commands(Config config, (void *) data);
/* Set wheter to log event additions; takes a bool pointer, cast to a void pointer. */
void configset_log_event_add(Config config, (void *) data);
/* Set wheter to log event callbacks; takes a bool pointer, cast to a void pointer. */
void configset_log_event_callbacks(Config config, (void *) data);
/* Set wheter to log keyframe execution; takes a bool pointer, cast to a void pointer. */
void configset_log_keyframes(Config config, (void *) data);

/* Set the PCA-9685's pin base; takes an int pointer, cast to a void pointer. */
void configset_pca_9685_pin_base(Config config, (void *) data);
/* Set the PCA-9685's max PWM value; takes an int pointer, cast to a void pointer. */
void configset_pca_9685_max_pwm(Config config, (void *) data);
/* Set the PCA-9685's frequency value; takes an int pointer, cast to a void pointer. */
void configset_pca_9685_hertz(Config config, (void *) data);

/* Set the number of robot servos; takes an int pointer, cast to a void pointer. */
void configset_servos_num(Config config, (void *) data);
/* Set speed of the robot's tick; takes a float pointer, cast to a void pointer. */
void configset_robot_tick(Config config, (void *) data);
/* Set whether to enable transition motions; takes a bool pointer, cast to a void pointer. */
void configset_transitions_enable(Config config, (void *) data);
/* Set the length of transition motions in seconds; takes a float pointer, cast to a void pointer. */
void configset_transition_time(Config config, (void *) data);

/* Set the magnitude of the hip during the walk motion; takes a float pointer, cast to a void pointer. */
void configset_walk_hip_delta(Config config, (void *) data);
/* Set the magnitude of the knee during the walk motion; takes a float pointer, cast to a void pointer. */
void configset_walk_knee_delta(Config config, (void *) data);
/* Set the first wait time for the knee during the walk motion; takes a float pointer, cast to a void pointer. */
void configset_walk_knee_pad_a(Config config, (void *) data);
/* Set the second wait time for the knee during the walk motion; takes a float pointer, cast to a void pointer. */
void configset_walk_knee_pad_b(Config config, (void *) data);

/* Set the servo pin value for a given robot position; takes a ServoPinData pointer, cast to a void pointer. */
void configset_servo_pins(Config config, (void *) data);
/* Set the servo PWM limits for a given robot position; takes a ServoLimitData pointer, cast to a void pointer. */
void configset_servo_limits(Config config, (void *) data);

#endif