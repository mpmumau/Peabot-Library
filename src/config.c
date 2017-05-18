#ifndef CONFIG_DEF
#define CONFIG_DEF

/*
 File:          config.c
 Description:   Implementation of config functions.
 Created:       May 8, 2017
 Author:        Matt Mumau
 */

#define _POSIX_C_SOURCE 199309L

/* System includes*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Application includes */
#include "config_defaults.h"
#include "config_callbacks.h"
#include "main.h"
#include "log.h"
#include "string_utils.h"

/* Header */
#include "config.h"

static Config config;

/* Forward decs */
static void config_set_defaults();
static void config_pipe(int argc, char *argv[]);
static void config_handle_arg(char *arg, char *val);
static char *config_default_log_filename();

void config_init(int argc, char *argv[])
{
    config_set_defaults();
    config_pipe(argc, argv);
}

void config_destroy()
{
    if (config.log_filename)
        free(config.log_filename);

    if (config.log_fullpath)
        free(config.log_fullpath);

    if (config.servo_pins)
        free(config.servo_pins);

    if (config.servo_limits)
        free(config.servo_limits);
}

static void config_set_defaults()
{
    char *log_file_dir = DEFAULT_LOG_DIR;
    config_set(CONF_LOG_FILE_DIR, (void *) log_file_dir);
    
    char *log_filename = config_default_log_filename();
    config_set(CONF_LOG_FILENAME, (void *) log_filename);

    int full_path_size = strlen(config.log_file_dir) + strlen(config.log_filename) + 1;
    config.log_fullpath = malloc(sizeof(char) * full_path_size);
    if (!config.log_fullpath)
        app_exit("[ERROR!] Unable to allocate memory for config.log_fullpath (config_set_defaults).", 1);

    strcpy(config.log_fullpath, config.log_file_dir);
    strcat(config.log_fullpath, config.log_filename);

    bool log_stdin = DEFAULT_LOG_STDIN;
    config_set(CONF_LOG_STDIN, (void *) &log_stdin);

    bool log_prompt_commands = DEFAULT_LOG_PROMPT_COMMANDS;
    config_set(CONF_LOG_PROMPT_COMMANDS, (void *) &log_prompt_commands);

    bool log_event_add = DEFAULT_LOG_EVENT_ADD;
    config_set(CONF_LOG_EVENT_ADD, (void *) &log_event_add);

    bool log_event_callbacks = DEFAULT_LOG_EVENT_CALLBACKS;
    config_set(CONF_LOG_EVENT_CALLBACKS, (void *) &log_event_callbacks);

    bool log_keyframes = DEFAULT_LOG_KEYFRAMES;
    config_set(CONF_LOG_KEYFRAMES, (void *) &log_keyframes);

    int pca_9685_pin_base = DEFAULT_PCA_9685_PIN_BASE;
    config_set(CONF_PCA_9685_PIN_BASE, (void *) &pca_9685_pin_base);

    int pca_9685_max_pwm = DEFAULT_PCA_9685_MAX_PWM;
    config_set(CONF_PCA_9685_MAX_PWM, (void *) &pca_9685_max_pwm);

    int pca_9685_hertz = DEFAULT_PCA_9685_HERTZ;
    config_set(CONF_PCA_9685_HERTZ, (void *) &pca_9685_hertz);

    int servos_num = DEFAULT_SERVOS_NUM;
    config_set(CONF_SERVOS_NUM, (void *) &servos_num);

    float robot_tick = DEFAULT_ROBOT_TICK;
    config_set(CONF_ROBOT_TICK, (void *) &robot_tick);

    bool transitions_enable = DEFAULT_TRANSITIONS_ENABLE;
    config_set(CONF_TRANSITIONS_ENABLE, (void *) &transitions_enable);

    float transitions_time = DEFAULT_KEYFRAME_TRANSITION_TIME;
    config_set(CONF_TRANSITIONS_TIME, (void *) &transitions_time);

    float walk_hip_delta = DEFAULT_HIP_DELTA;
    config_set(CONF_WALK_HIP_DELTA, (void *) &walk_hip_delta);

    float walk_knee_delta = DEFAULT_KNEE_DELTA;
    config_set(CONF_WALK_KNEE_DELTA, (void *) &walk_knee_delta);

    float walk_knee_pad_a = DEFAULT_KNEE_PAD_A;
    config_set(CONF_WALK_KNEE_PAD_A, (void *) &walk_knee_pad_a);

    float walk_knee_pad_b = DEFAULT_KNEE_PAD_B;
    config_set(CONF_WALK_KNEE_PAD_B, (void *) &walk_knee_pad_b);

    // Do these after processing other configs; dependent upon them.
    config.servo_pins = malloc(sizeof(int) * config.servos_num);
    if (!config.servo_pins)
        app_exit("[ERROR!] Could not allocate memory for config.servo_pins (config_init).", 1);

    ServoPinData servo_pin_data;
    int num;
    
    for (int i = 0; i < config.servos_num; i++)
    {  
        num = -1;

        switch (i)
        {
            case SERVO_INDEX_BACK_LEFT_KNEE:
                num = DEFAULT_BACK_LEFT_KNEE;
                break;
            case SERVO_INDEX_BACK_LEFT_HIP:
                num = DEFAULT_BACK_LEFT_HIP;
                break;
            case SERVO_INDEX_FRONT_LEFT_KNEE:
                num = DEFAULT_FRONT_LEFT_KNEE;
                break;                
            case SERVO_INDEX_FRONT_LEFT_HIP:
                num = DEFAULT_FRONT_LEFT_HIP;
                break;
            case SERVO_INDEX_BACK_RIGHT_KNEE:
                num = DEFAULT_BACK_RIGHT_KNEE;
                break;
            case SERVO_INDEX_BACK_RIGHT_HIP:
                num = DEFAULT_BACK_RIGHT_HIP;
                break;
            case SERVO_INDEX_FRONT_RIGHT_KNEE:
                num = DEFAULT_FRONT_RIGHT_KNEE;
                break;     
            case SERVO_INDEX_FRONT_RIGHT_HIP:
                num = DEFAULT_FRONT_RIGHT_HIP;
                break;       
            default:
                continue;       
        }

        if (num < 0)
            continue;

        servo_pin_data.id = i;
        servo_pin_data.val = num;
        config_set(CONF_SERVO_PINS, (void *) &servo_pin_data);
    }

    config.servo_limits = malloc(sizeof(ServoLimit) * config.servos_num);
    if (!config.servo_pins)
        app_exit("[ERROR!] Could not allocate memory for config.servo_limits (config_init).", 1);

    ServoLimitData servo_limit_data;

    for (int j = 0; j < config.servos_num; j++)
    {
        servo_limit_data.id = j;
        servo_limit_data.min = DEFAULT_SERVO_DEFAULT_MIN;
        servo_limit_data.max = DEFAULT_SERVO_DEFAULT_MAX;

        config_set(CONF_SERVO_LIMITS, (void *) &servo_limit_data);
    }    
}

static void config_pipe(int argc, char *argv[])
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

void config_set(int config_var, void *data)
{
    void (*config_set_callback)(Config *config, void *data);
    config_set_callback = NULL;

    if (config_var == CONF_LOG_FILE_DIR)
        config_set_callback = configset_log_file_dir;

    if (config_var == CONF_LOG_FILENAME) 
        config_set_callback = configset_log_filename;

    if (config_var == CONF_LOG_STDIN) 
        config_set_callback = configset_log_stdin; 

    if (config_var == CONF_LOG_PROMPT_COMMANDS)
        config_set_callback = configset_log_prompt_commands;

    if (config_var == CONF_LOG_EVENT_ADD) 
        config_set_callback = configset_log_event_add; 

    if (config_var == CONF_LOG_EVENT_CALLBACKS) 
        config_set_callback = configset_log_event_callbacks; 

    if (config_var == CONF_LOG_KEYFRAMES) 
        config_set_callback = configset_log_keyframes;      

    if (config_var == CONF_PCA_9685_PIN_BASE) 
        config_set_callback = configset_pca_9685_pin_base;           

    if (config_var == CONF_PCA_9685_MAX_PWM) 
        config_set_callback = configset_pca_9685_max_pwm;      

    if (config_var == CONF_PCA_9685_HERTZ) 
        config_set_callback = configset_pca_9685_hertz;           

    if (config_var == CONF_SERVOS_NUM) 
        config_set_callback = configset_servos_num;       

    if (config_var == CONF_ROBOT_TICK) 
        config_set_callback = configset_robot_tick;      

    if (config_var == CONF_TRANSITIONS_ENABLE) 
        config_set_callback = configset_transitions_enable;    

    if (config_var == CONF_TRANSITIONS_TIME) 
        config_set_callback = configset_transition_time;       

    if (config_var == CONF_SERVO_PINS) 
        config_set_callback = configset_servo_pins;  

    if (config_var == CONF_SERVO_LIMITS) 
        config_set_callback = configset_servo_limits;    

    if (config_var == CONF_WALK_HIP_DELTA) 
        config_set_callback = configset_walk_hip_delta;         

    if (config_var == CONF_WALK_KNEE_DELTA) 
        config_set_callback = configset_walk_knee_delta; 

    if (config_var == CONF_WALK_KNEE_PAD_A) 
        config_set_callback = configset_walk_knee_pad_a;         

    if (config_var == CONF_WALK_KNEE_PAD_B) 
        config_set_callback = configset_walk_knee_pad_b;     

    /* Callback execute */
    if (config_set_callback != NULL)
        (*config_set_callback)(&config, data);
}

void *config_get(int config_var)
{
    void *ret_val;

    if (config_var == CONF_LOG_FILE_DIR)
        ret_val = (void *) config.log_file_dir;

    if (config_var == CONF_LOG_FILENAME)
        ret_val = (void *) config.log_filename;    

    if (config_var == CONF_LOG_FULLPATH)
        ret_val = (void *) config.log_fullpath;    

    if (config_var == CONF_CONFIG_FILE)
        ret_val = (void *) config.config_file;  

    if (config_var == CONF_LOG_STDIN)
        ret_val = (void *) &(config.log_stdin);      

    if (config_var == CONF_LOG_PROMPT_COMMANDS)
        ret_val = (void *) &(config.log_prompt_commands);     
        
     if (config_var == CONF_LOG_EVENT_ADD)
        ret_val = (void *) &(config.log_event_add);  
        
     if (config_var == CONF_LOG_EVENT_CALLBACKS)
        ret_val = (void *) &(config.log_event_callbacks);   

     if (config_var == CONF_LOG_KEYFRAMES)
        ret_val = (void *) &(config.log_keyframes);       

     if (config_var == CONF_PCA_9685_PIN_BASE)
        ret_val = (void *) &(config.pca_9685_pin_base);  

     if (config_var == CONF_PCA_9685_MAX_PWM)
        ret_val = (void *) &(config.pca_9685_max_pwm);  

     if (config_var == CONF_PCA_9685_HERTZ)
        ret_val = (void *) &(config.pca_9685_hertz);

     if (config_var == CONF_SERVOS_NUM)
        ret_val = (void *) &(config.servos_num);

     if (config_var == CONF_ROBOT_TICK)
        ret_val = (void *) &(config.robot_tick);   
        
     if (config_var == CONF_TRANSITIONS_ENABLE)
        ret_val = (void *) &(config.transitions_enable);  
        
     if (config_var == CONF_TRANSITIONS_TIME)
        ret_val = (void *) &(config.transition_time);                

     if (config_var == CONF_TRANSITIONS_TIME)
        ret_val = (void *) &(config.transition_time); 

     if (config_var == CONF_SERVO_PINS)
        ret_val = (void *) config.servo_pins;       
        
     if (config_var == CONF_SERVO_LIMITS)
        ret_val = (void *) config.servo_limits;   
        
     if (config_var == CONF_WALK_HIP_DELTA)
        ret_val = (void *) &(config.walk_hip_delta);

     if (config_var == CONF_WALK_KNEE_DELTA)
        ret_val = (void *) &(config.walk_knee_delta);           

     if (config_var == CONF_WALK_KNEE_PAD_A)
        ret_val = (void *) &(config.walk_knee_pad_a);

     if (config_var == CONF_WALK_KNEE_PAD_B)
        ret_val = (void *) &(config.walk_knee_pad_b);                                                                                                                                            

    return ret_val;
}

static void config_handle_arg(char *arg, char *val)
{
    if (str_equals(arg, "c")) {
        // handle config file
    }

    if (str_equals(arg, "rt"))
    {
        // handle setting the robot tick speed from the command line
    }    
}

static char *config_default_log_filename()
{
    char *filename = malloc(sizeof(char) * LOG_FILENAME_MAXLEN);

    time_t current_time = time(NULL);
    struct tm *ltime = localtime(&current_time);

    snprintf(filename, LOG_FILENAME_MAXLEN, 
        "peabot.%.*d-%.*d-%.*d_%.*d-%.*d-%.*d.log", 
        2,
        ltime->tm_mon, 
        2,
        ltime->tm_mday, 
        2,
        ltime->tm_year % 100, 
        2,
        ltime->tm_hour, 
        2,
        ltime->tm_min, 
        2,
        ltime->tm_sec);  

    return filename;
}

#endif