#ifndef ROBOT_DEF
#define ROBOT_DEF

/*
 File:          robot.c
 Description:   Implementations of Peabot robot functions.
 Created:       May 7, 2017
 Author:        Matt Mumau
 */

#define _POSIX_C_SOURCE 199309L

/* System includes */
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

/* Raspberry Pi libraries */
#include <wiringPi.h>
#include <pca9685.h>

/* Application includes */
#include "main.h"
#include "config.h"
#include "log.h"
#include "utils.h"

/* Header */
#include "robot.h"

static pthread_t robot_thread;
static bool running = true;

static int pca_9685_fd;

static float servo[SERVOS_NUM];
static ServoLimit *servo_limits;

/* Forward decs */
static void *robot_main(void *arg);
static void robot_mvjoint(int joint, float val);
static bool robot_jointinv(int joint);
static int robot_mapsrv(float val, int min, int max);
static void robot_destroy();

void robot_init()
{
    wiringPiSetup();

    pca_9685_fd = pca9685Setup(PCA_9685_PIN_BASE, 0x40, PCA_9685_HERTZ);
    if (pca_9685_fd < 0)
        app_exit("[ERROR!] Could not create PCA-9685 file descriptor.", 1);
    pca9685PWMReset(pca_9685_fd);

    int error = pthread_create(&robot_thread, NULL, robot_main, NULL);
    if (error)
        app_exit("[ERROR!] Could not initialize robot thread.", 1);

    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);
    ServoLimit *servo_limits_conf = (ServoLimit *) config_get(CONF_SERVO_LIMITS);

    servo_limits = malloc(sizeof(ServoLimit) * (*servos_num));
    for (int i = 0; i < *servos_num; i++)
    {
        servo_limits[i]->min = servo_limits_conf[i]->min;
        servo_limits[i]->max = servo_limits_conf[i]->max;
    }
}

void robot_halt()
{   
    robot_reset();

    robot_destroy();

    running = false;
    int error = pthread_join(robot_thread, NULL);
    if (error)
        log_event("[ERROR!] Could not rejoin from robot thread.");

    pca9685PWMReset(pca_9685_fd);
}

void robot_reset()
{
    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);

    for (int i = 0; i < *servos_num; i++)
        servo[i] = 0.0f; 
}

void robot_setservo(int pin, float val)
{
    servo[pin] = val;
}

void robot_set_servo_limit(int pin, int min, int max)
{
    servo_limits[pin].min = min;
    servo_limits[pin].max = max;
}

float robot_getservo(int pin)
{
    return servo[pin];
}

static void *robot_main(void *arg)
{
    struct timespec time;
    struct timespec last_time;

    float tick = 0.0f;
    float diff;
    float *robot_tick = (float *) config_get(CONF_ROBOT_TICK);
    int *servos_num = (int *) config_get(CONF_SERVOS_NUM);

    while (running)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        diff = utils_timediff(time, last_time);
        last_time = time;
        
        tick += diff;
        if (tick < *robot_tick)
            continue;

        tick = 0.0f;
        for (int i = 0; i < *servos_num; i++)
        {
           robot_mvjoint(i, servo[i]); 
        }
    }

    return (void *) NULL;
}

static void robot_mvjoint(int joint, float val)
{
    if (robot_jointinv(joint))
        val *= -1.0f;

    int *pin_data = (int *) config_get(CONF_SERVO_PINS);
    int pin = pin_data[joint];

    ServoLimit *servo_limit = &servo_limits[joint];

    int mapped_val = robot_mapsrv(val, servo_limit->min, servo_limit->max);
    pwmWrite(PCA_9685_PIN_BASE + pin, mapped_val);
}

static bool robot_jointinv(int joint)
{
    if (joint == FRONT_LEFT_KNEE ||
        joint == FRONT_LEFT_HIP ||
        joint == BACK_RIGHT_KNEE ||
        joint == BACK_RIGHT_HIP)
        return true;

    return false;
}

static int robot_mapsrv(float val, int min, int max)
{
    if (min >= max)
        return -1;

    if (val > 1.0f)
        val = 1.0f;
    if (val < -1.0f)
        val = -1.0f;

    float abs = fabs(val);
    int half = (max - min) / 2;
    int midway = min + half;
    int diff = half * abs;

    if (val < 0)
        return midway - diff;
    
    return midway + diff;
}

static void robot_destroy()
{
    if (servo_limits)
        free(servo_limits);
}

#endif