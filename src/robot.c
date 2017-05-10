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
#include "config.h"
#include "main.h"
#include "utils.h"

#include "robot.h"

static pthread_t robot_thread;
static bool robot_running = true;

/* Device globals */
static int pca_9685_fd;

/* Servo values*/
static ServoLimit servo_limits[SERVOS_NUM];
static float servo[SERVOS_NUM];

/* Forward decs */
static void *robot_main(void *arg);
static int robot_mapsrv(float val, int min, int max);
static bool robot_jointinv(int joint);
static void robot_mvjoint(int joint, float val);

void robot_init()
{
    wiringPiSetup();

    pca_9685_fd = pca9685Setup(PCA_9685_PIN_BASE, 0x40, PCA_9685_HERTZ);
    if (pca_9685_fd < 0)
        app_exit("[ERROR!] Could not create PCA-9685 file descriptor.", 1);

    pca9685PWMReset(pca_9685_fd);

    pthread_create(&robot_thread, NULL, robot_main, NULL);
}

void robot_halt()
{   
    robot_reset();
    robot_running = false;
    pthread_join(robot_thread, NULL);
    pca9685PWMReset(pca_9685_fd);
}

void robot_reset()
{
    for (int i = 0; i < SERVOS_NUM; i++)
        servo[i] = 0.0f; 
}

void robot_set_servo_limit(int pin, int min, int max)
{
    servo_limits[pin].min = min;
    servo_limits[pin].max = max;
}

void robot_setservo(int pin, float val)
{
    servo[pin] = val;
}

float robot_getservo(int pin)
{
    return servo[pin];
}

static void *robot_main(void *arg)
{
    float tick = ROBOT_TICK;

    struct timespec time;
    struct timespec last_time;

    while (robot_running)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        float diff = utils_timediff(time, last_time);
        last_time = time;
        tick -= diff;

        if (tick > 0.0f)
            continue;

        tick = ROBOT_TICK;

        for (int i = 0; i < SERVOS_NUM; i++)
        {
           robot_mvjoint(i, servo[i]); 
        }
    }

    return (void *) NULL;
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

static bool robot_jointinv(int joint)
{
    if (joint == FRONT_LEFT_KNEE ||
        joint == FRONT_LEFT_HIP ||
        joint == BACK_RIGHT_KNEE ||
        joint == BACK_RIGHT_HIP)
        return true;

    return false;
}

static void robot_mvjoint(int joint, float val)
{
    if (robot_jointinv(joint))
        val *= -1.0f;

    int mapped_val = robot_mapsrv(val, 200, 400);
    pwmWrite(PCA_9685_PIN_BASE + joint, mapped_val);
}

#endif