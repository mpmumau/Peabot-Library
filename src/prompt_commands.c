#ifndef PROMPT_COMMANDS_DEF
#define PROMPT_COMMANDS_DEF

/*
 File:          prompt_commands.c
 Description:   Implementation of prompt command callback functions.
 Created:       May 9, 2017
 Author:        Matt Mumau
 */

/* System includes */ 
#include <stdlib.h>
#include <stdio.h>

/* Application includes */
#include "config_defaults.h"
#include "main.h"
#include "config.h"
#include "log.h"
#include "console.h"
#include "events.h"
#include "string_utils.h"

/* Header */
#include "prompt_commands.h"

void promptcmd_quit(char *args[], int arg_num)
{
    app_exit("[Prompt] User requested application shutdown.", 0);
}


void promptcmd_reset(char *args[], int arg_num)
{
    bool *log_prompt_commands = (bool *) config_get(CONF_LOG_PROMPT_COMMANDS);
    if (*log_prompt_commands)
    {
        char log_msg[LOG_LINE_MAXLEN];
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Prompt] Adding reset event.");
        log_event(log_msg);
    } 

    event_add(EVENT_RESET, (void *) NULL);  
}

void promptcmd_delay(char *args[], int arg_num)
{
    if (arg_num != 1)
    {
        console_print("[ERROR] Incorrect number of params. Usage: delay [duration]");
        return;
    }

    const char *seconds_string = args[0];
    float *seconds = calloc(1, sizeof(float));
    *seconds = (float) atof(seconds_string);

    bool *log_prompt_commands = (bool *) config_get(CONF_LOG_PROMPT_COMMANDS);
    if (*log_prompt_commands)
    {
        char log_msg[LOG_LINE_MAXLEN];
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Prompt] Adding delay event. (duration: %f)", *seconds);
        log_event(log_msg);
    } 

    event_add(EVENT_DELAY, (void *) seconds);    
}

void promptcmd_elevate(char *args[], int arg_num)
{
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: elevate [duration] [reverse]");
        return;
    }

    const char *seconds_string = args[0];
    const char *reverse_string = args[1];

    EventElevateData *elevate_data = calloc(1, sizeof(EventElevateData));
    elevate_data->duration = (float) atof(seconds_string);
    elevate_data->reverse = (bool) ((int) atoi(reverse_string));
    
    bool *log_prompt_commands = (bool *) config_get(CONF_LOG_PROMPT_COMMANDS);
    if (*log_prompt_commands)
    {
        char log_msg[LOG_LINE_MAXLEN];
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Prompt] Adding elevate event. (duration: %f, reverse %d)", elevate_data->duration, elevate_data->reverse);
        log_event(log_msg);
    } 

    event_add(EVENT_ELEVATE, (void *) elevate_data);    
}

void promptcmd_extend(char *args[], int arg_num)
{
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: extend [duration] [reverse]");
        return;
    }

    const char *seconds_string = args[0];
    const char *reverse_string = args[1];

    EventExtendData *extend_data = calloc(1, sizeof(EventExtendData));
    extend_data->duration = (float) atof(seconds_string);
    extend_data->reverse = (bool) ((int) atoi(reverse_string));
    
    bool *log_prompt_commands = (bool *) config_get(CONF_LOG_PROMPT_COMMANDS);
    if (*log_prompt_commands)
    {
        char log_msg[LOG_LINE_MAXLEN];
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Prompt] Adding extend event. (duration: %f, reverse %d)", extend_data->duration, extend_data->reverse);
        log_event(log_msg);
    } 

    event_add(EVENT_EXTEND, (void *) extend_data);    
}

void promptcmd_walk(char *args[], int arg_num)
{
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: walk [cycles] [duration]");
        return;
    }

    const char *cycles_string = args[0];
    const char *seconds_string = args[1];

    EventWalkData *walk_data = calloc(1, sizeof(EventWalkData));
    walk_data->cycles = atoi(cycles_string);
    walk_data->duration = atof(seconds_string);

    bool *log_prompt_commands = (bool *) config_get(CONF_LOG_PROMPT_COMMANDS);
    if (*log_prompt_commands)
    {
        char log_msg[LOG_LINE_MAXLEN];
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Prompt] Adding walk event. (duration: %f, cycles %d)", walk_data->duration, walk_data->cycles);
        log_event(log_msg);
    }     

    event_add(EVENT_WALK, (void *) walk_data);
}

void promptcmd_cfg_get(char *args[], int arg_num)
{
    if (arg_num < 1)
    {
        console_print("[ERROR] Incorrect number of params. Usage: cfg_get [variable] [other_data]");
        return;
    }

    char *var_name = args[0];

    if (str_equals(var_name, "log_file_dir"))
    {
        char *log_file_dir = (char *) config_get(CONF_LOG_FILE_DIR);
        printf("[Config] log_file_dir: %s\n", log_file_dir);
        return;
    }

    if (str_equals(var_name, "log_filename"))
    {
        char *log_filename = (char *) config_get(CONF_LOG_FILENAME);
        printf("[Config] log_filename: %s\n", log_filename);
        return;
    }   

    if (str_equals(var_name, "log_fullpath"))
    {
        char *log_fullpath = (char *) config_get(CONF_LOG_FULLPATH);
        printf("[Config] log_fullpath: %s\n", log_fullpath);
        return;
    } 

    if (str_equals(var_name, "config_file"))
    {
        char *config_file = (char *) config_get(CONF_CONFIG_FILE);
        printf("[Config] config_file: %s\n", config_file);
        return;
    } 

    if (str_equals(var_name, "log_stdin"))
    {
        bool *val = (bool *) config_get(CONF_LOG_STDIN);
        char *truth = *val ? "true" : "false";
        printf("[Config] log_stdin: %s\n", truth);
        return;
    } 

    if (str_equals(var_name, "log_prompt_commands"))
    {
        bool *val = (bool *) config_get(CONF_LOG_PROMPT_COMMANDS);
        char *truth = *val ? "true" : "false";
        printf("[Config] log_prompt_commands: %s\n", truth);
        return;
    }    

    if (str_equals(var_name, "log_event_add"))
    {
        bool *val = (bool *) config_get(CONF_LOG_EVENT_ADD);
        char *truth = *val ? "true" : "false";
        printf("[Config] log_event_add: %s\n", truth);
        return;
    }   

    if (str_equals(var_name, "log_event_callbacks"))
    {
        bool *val = (bool *) config_get(CONF_LOG_EVENT_CALLBACKS);
        char *truth = *val ? "true" : "false";
        printf("[Config] log_event_callbacks: %s\n", truth);
        return;
    }  

    if (str_equals(var_name, "log_keyframes"))
    {
        bool *val = (bool *) config_get(CONF_LOG_KEYFRAMES);
        char *truth = *val ? "true" : "false";
        printf("[Config] log_keyframes: %s\n", truth);
        return;
    }     

    if (str_equals(var_name, "pca_9685_pin_base"))
    {
        int *val = (int *) config_get(CONF_PCA_9685_PIN_BASE);
        printf("[Config] pca_9685_pin_base: %i\n", *val);
        return;
    }    

    if (str_equals(var_name, "pca_9685_max_pwm"))
    {
        int *val = (int *) config_get(CONF_PCA_9685_MAX_PWM);
        printf("[Config] pca_9685_max_pwm: %i\n", *val);
        return;
    }          

    if (str_equals(var_name, "pca_9685_hertz"))
    {
        int *val = (int *) config_get(CONF_PCA_9685_HERTZ);
        printf("[Config] pca_9685_hertz: %i\n", *val);
        return;
    }      

    if (str_equals(var_name, "servos_num"))
    {
        int *val = (int *) config_get(CONF_SERVOS_NUM);
        printf("[Config] servos_num: %i\n", *val);
        return;
    }        

    if (str_equals(var_name, "robot_tick"))
    {
        float *val = (float *) config_get(CONF_ROBOT_TICK);
        printf("[Config] robot_tick: %f\n", *val);
        return;
    }    

    if (str_equals(var_name, "transitions_enable"))
    {
        bool *val = (bool *) config_get(CONF_TRANSITIONS_ENABLE);
        char *truth = *val ? "true" : "false";
        printf("[Config] transitions_enable: %s\n", truth);
        return;
    } 

    if (str_equals(var_name, "transitions_time"))
    {
        float *val = (float *) config_get(CONF_TRANSITIONS_TIME);
        printf("[Config] transitions_time: %f\n", *val);
        return;
    }             

    if (str_equals(var_name, "walk_hip_delta"))
    {
        float *val = (float *) config_get(CONF_WALK_HIP_DELTA);
        printf("[Config] walk_hip_delta: %f\n", *val);
        return;
    }       

    if (str_equals(var_name, "walk_knee_delta"))
    {
        float *val = (float *) config_get(CONF_WALK_KNEE_DELTA);
        printf("[Config] walk_knee_delta: %f\n", *val);
        return;
    }       

    if (str_equals(var_name, "walk_knee_pad_a"))
    {
        float *val = (float *) config_get(CONF_WALK_KNEE_PAD_A);
        printf("[Config] walk_knee_pad_a: %f\n", *val);
        return;
    }               

    if (str_equals(var_name, "walk_knee_pad_b"))
    {
        float *val = (float *) config_get(CONF_WALK_KNEE_PAD_B);
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

        int index = (int) atoi(args[1]);
        int *pins = config_get(CONF_SERVO_PINS);

        int *servos_num = (int *) config_get(CONF_SERVOS_NUM);
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

        int index = (int) atoi(args[1]);

        int *servos_num = (int *) config_get(CONF_SERVOS_NUM);
        if (index > *servos_num - 1)
        {
            console_print("[ERROR] Provided index is greater than the number of servos.");
            return;
        }        

        ServoLimit *servo_limits = (ServoLimit *) config_get(CONF_SERVO_LIMITS);        

        printf("[Config] servo_limit@%d: %d-%d\n", index, servo_limits[index].min, servo_limits[index].max);
        return;
    }

    console_print("[ERROR] Could not get config variable with specified name.");
    return;
}

void promptcmd_turn(char *args[], int arg_num)
{
    if (arg_num != 2)
    {
        console_print("[ERROR] Incorrect number of params. Usage: turn [cycles] [duration]");
        return;
    }

    const char *cycles_string = args[0];
    const char *seconds_string = args[1];    

    int cycles = (int) atoi(cycles_string);
    float duration = (float) atof(seconds_string);

    EventTurnData *turn_data = calloc(1, sizeof(EventTurnData));
    turn_data->cycles = cycles;
    turn_data->duration = duration;

    bool *log_prompt_commands = (bool *) config_get(CONF_LOG_PROMPT_COMMANDS);
    if (*log_prompt_commands)
    {
        char log_msg[LOG_LINE_MAXLEN];
        snprintf(log_msg, LOG_LINE_MAXLEN, "[Prompt] Adding turn event. (duration: %f, cycles %d)", turn_data->duration, turn_data->cycles);
        log_event(log_msg);
    }     

    event_add(EVENT_TURN, (void *) turn_data);    
}

#endif