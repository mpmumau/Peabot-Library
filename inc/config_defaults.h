#ifndef CONFIG_DEFAULTS_H_DEF
#define CONFIG_DEFAULTS_H_DEF

/*
 File:          config_defaults.h
 Description:   Default values for configuration variables.
 Created:       May 15, 2017
 Author:        Matt Mumau
 */

/* Log settings */
#define DEFAULT_LOG_DIR "/var/log/peabot/"
#define DEFAULT_LOG_STDIN 1
#define DEFAULT_LOG_PROMPT_COMMANDS 1
#define DEFAULT_LOG_EVENT_ADD 1
#define DEFAULT_LOG_EVENT_CALLBACKS 1
#define DEFAULT_LOG_KEYFRAMES 1

/* PCA-9685 config */
#define DEFAULT_PCA_9685_PIN_BASE 300
#define DEFAULT_PCA_9685_MAX_PWM 4096
#define DEFAULT_PCA_9685_HERTZ 50

/* Robot related*/
#define DEFAULT_SERVOS_NUM 8
#define DEFAULT_ROBOT_TICK 0.01
#define DEFAULT_TRANSITIONS_ENABLE 1
#define DEFAULT_KEYFRAME_TRANSITION_TIME 1.0

/* Walk related */
#define DEFAULT_HIP_DELTA 0.7
#define DEFAULT_KNEE_DELTA 0.3
#define DEFAULT_KNEE_PAD_A 0.9
#define DEFAULT_KNEE_PAD_B 0.9

/* PCA_9685 servo pins; left/right relative to the robot. */
#define DEFAULT_FRONT_RIGHT_KNEE 6
#define DEFAULT_FRONT_RIGHT_HIP 7
#define DEFAULT_FRONT_LEFT_KNEE 3
#define DEFAULT_FRONT_LEFT_HIP 2
#define DEFAULT_BACK_RIGHT_KNEE 4
#define DEFAULT_BACK_RIGHT_HIP 5
#define DEFAULT_BACK_LEFT_KNEE 0
#define DEFAULT_BACK_LEFT_HIP 1

/* Servo limits */
#define DEFAULT_SERVO_DEFAULT_MIN 200
#define DEFAULT_SERVO_DEFAULT_MAX 400

#endif