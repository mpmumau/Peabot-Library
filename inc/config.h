/*
 File:          config.h
 Description:   Peabot application-level configuration.
 */

#ifndef CONFIG_H_DEF
#define CONFIG_H_DEF

#include <stdbool.h>
#include "robot.h"

enum ConfigFlag {
    CONF_LOG_FILE_DIR,
    CONF_LOG_FILENAME,
    CONF_LOG_FULLPATH,

    CONF_CONFIG_FILE,

    CONF_LOG_STDIN,
    CONF_LOG_PROMPT_COMMANDS,
    CONF_LOG_EVENT_ADD,
    CONF_LOG_EVENT_CALLBACKS,
    CONF_LOG_KEYFRAMES,

    CONF_PCA_9685_PIN_BASE,
    CONF_PCA_9685_MAX_PWM,
    CONF_PCA_9685_HERTZ,

    CONF_SERVOS_NUM,
    CONF_ROBOT_TICK,
    CONF_TRANSITIONS_ENABLE,
    CONF_TRANSITIONS_TIME,
    CONF_SERVO_PINS,
    CONF_SERVO_LIMITS,

    CONF_WALK_HIP_DELTA,
    CONF_WALK_KNEE_DELTA,
    CONF_WALK_KNEE_PAD_A,
    CONF_WALK_KNEE_PAD_B,

    CONF_HTTP_ENABLED,
    CONF_HTTP_PORT
};

/* Config data struct */
typedef struct Config {
    const char *log_file_dir;
    const char *log_filename;
    const char *log_fullpath;

    const char *config_file;

    bool log_stdin;
    bool log_prompt_commands;
    bool log_event_add;
    bool log_event_callbacks;
    bool log_keyframes;

    unsigned int pca_9685_pin_base;
    unsigned int pca_9685_max_pwm;
    unsigned int pca_9685_hertz;

    unsigned short servos_num;
    double robot_tick;
    bool transitions_enable;
    double transition_time;
    
    unsigned short *servo_pins;
    ServoLimit *servo_limits;

    double walk_hip_delta;
    double walk_knee_delta;
    double walk_knee_pad_a;
    double walk_knee_pad_b;

    bool http_enabled;
    unsigned short http_port;
} Config;

typedef struct ServoPinData {
    unsigned short id;
    unsigned short val;
} ServoPinData;

typedef struct ServoLimitData {
    unsigned short id;
    unsigned short min;
    unsigned short max;
} ServoLimitData;

/* Initialize the application configuration, setting all variables. */
void config_init(int argc, char *argv[]);

/* Free memory allocated on the stack for config variables. */
void config_destroy();

/* Set the variable correlating the identifier (a value from the ConfigFlag enum) to a data object. */ 
void config_set(unsigned short config_var, void *data, bool is_string);

/* Return a generic data object representing the value of the given configuration var identifier. */
void *config_get(unsigned short config_var);

/* Get the integer value of a servo index from a string. */
unsigned short config_str_to_servo_index(const char *str);

#endif