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

static ServoLimit servo_limits[SERVOS_NUM];
static float servo[SERVOS_NUM];

/* Forward decs */
static void *robot_main(void *arg);
static void robot_mvjoint(int joint, float val);
static bool robot_jointinv(int joint);
static int robot_mapsrv(float val, int min, int max);

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
}

void robot_halt()
{   
    robot_reset();

    running = false;
    int error = pthread_join(robot_thread, NULL);
    if (error)
        log_event("[ERROR!] Could not rejoin from robot thread.");

    pca9685PWMReset(pca_9685_fd);
}

void robot_reset()
{
    for (int i = 0; i < SERVOS_NUM; i++)
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

    while (running)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        diff = utils_timediff(time, last_time);
        last_time = time;
        
        tick += diff;
        if (tick < ROBOT_TICK)
            continue;

        tick = 0.0f;
        for (int i = 0; i < SERVOS_NUM; i++)
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

    int mapped_val = robot_mapsrv(val, 200, 400);
    pwmWrite(PCA_9685_PIN_BASE + joint, mapped_val);
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

#endif