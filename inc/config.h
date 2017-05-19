#ifndef CONFIG_H_DEF
#define CONFIG_H_DEF

/*
 File:          config.h
 Description:   Peabot application-level configuration.
 Created:       May 5, 2017
 Author:        Matt Mumau
 */

#include <stdbool.h>

#include "robot.h"

enum ConfigFlag {
    CONF_LOG_FILE_DIR            = 1 << 0,
    CONF_LOG_FILENAME            = 1 << 1,
    CONF_LOG_FULLPATH            = 1 << 2,

    CONF_CONFIG_FILE             = 1 << 3,

    CONF_LOG_STDIN               = 1 << 4,
    CONF_LOG_PROMPT_COMMANDS     = 1 << 5,
    CONF_LOG_EVENT_ADD           = 1 << 6,
    CONF_LOG_EVENT_CALLBACKS     = 1 << 7,
    CONF_LOG_KEYFRAMES           = 1 << 8,

    CONF_PCA_9685_PIN_BASE       = 1 << 9,
    CONF_PCA_9685_MAX_PWM        = 1 << 10,
    CONF_PCA_9685_HERTZ          = 1 << 11,

    CONF_SERVOS_NUM              = 1 << 12,
    CONF_ROBOT_TICK              = 1 << 13,
    CONF_TRANSITIONS_ENABLE      = 1 << 14,
    CONF_TRANSITIONS_TIME        = 1 << 15,
    CONF_SERVO_PINS              = 1 << 16,
    CONF_SERVO_LIMITS            = 1 << 17,

    CONF_WALK_HIP_DELTA          = 1 << 18,
    CONF_WALK_KNEE_DELTA         = 1 << 19,
    CONF_WALK_KNEE_PAD_A         = 1 << 20,
    CONF_WALK_KNEE_PAD_B         = 1 << 21    
};

/* Config data struct */
typedef struct Config {
    char *log_file_dir;
    char *log_filename;
    char *log_fullpath;

    char *config_file;

    bool log_stdin;
    bool log_prompt_commands;
    bool log_event_add;
    bool log_event_callbacks;
    bool log_keyframes;

    int pca_9685_pin_base;
    int pca_9685_max_pwm;
    int pca_9685_hertz;

    int servos_num;
    float robot_tick;
    bool transitions_enable;
    float transition_time;
    int *servo_pins;
    ServoLimit *servo_limits;

    float walk_hip_delta;
    float walk_knee_delta;
    float walk_knee_pad_a;
    float walk_knee_pad_b;
} Config;

typedef struct ServoPinData {
    int id;
    int val;
} ServoPinData;

typedef struct ServoLimitData {
    int id;
    int min;
    int max;
} ServoLimitData;

/* Initialize the application configuration, setting all variables. */
void config_init(int argc, char *argv[]);

/* Set the variable correlating the identifier (a value from the ConfigFlag enum) to a data object. */ 
void config_set(int config_var, void *data);

/* Return a generic data object representing the value of the given configuration var identifier. */
void *config_get(int config_var);

/* Get the integer value of a servo index from a string. */
int config_str_to_servo_index(char *str);

/* Free memory allocated on the stack for config variables. */
void config_destroy();

#endif