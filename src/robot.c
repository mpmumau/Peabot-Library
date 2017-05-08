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

#include "robot.h"

/* Internal thread */
pthread_t robot_thread;
bool robot_running = true;
struct timespec current_time;
struct timespec last_time;

/* Device globals */
int pca_9685_fd;

/* Servo values*/
struct ServoRange servo_ranges[SERVOS_NUM];
float servo[SERVOS_NUM];

/* Forward decs */
void *robot_tick(void *arg);
int robot_mapsrv(float val, int min, int max);
bool robot_jointinv(int joint);
void robot_mvjoint(int joint, float val);
void robot_reset();

void robot_init()
{
    wiringPiSetup();

    pca_9685_fd = pca9685Setup(PCA_9685_PIN_BASE, 0x40, PCA_9685_HERTZ);
    if (pca_9685_fd < 0)
    {
        app_exit("[ERROR!] Could not create PCA-9685 file descriptor.", 1);
    }

    pca9685PWMReset(pca_9685_fd);

    pthread_create(&robot_thread, NULL, robot_tick, NULL);
}

void robot_set_servo_limit(int pin, int min, int max)
{
    servo_ranges[pin].min = min;
    servo_ranges[pin].max = max;
}

void robot_halt()
{   
    robot_reset();
    robot_running = false;
    pthread_join(robot_thread, NULL);
    pca9685PWMReset(pca_9685_fd);
}

void robot_setservo(int pin, float val)
{
    servo[pin] = val;
}

void *robot_tick(void *arg)
{
    float tick = ROBOT_TICK;
    clock_gettime(CLOCK_MONOTONIC, &last_time);

    while (robot_running)
    {
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        float diff = ((float) current_time.tv_sec - (float) last_time.tv_sec) +
            ((float) current_time.tv_nsec - (float) last_time.tv_nsec) / 1000000000.0f;
        last_time = current_time;
        tick = tick - diff;

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

int robot_mapsrv(float val, int min, int max)
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

bool robot_jointinv(int joint)
{
    if (joint == FRONT_LEFT_KNEE ||
        joint == FRONT_LEFT_HIP ||
        joint == BACK_RIGHT_KNEE ||
        joint == BACK_RIGHT_HIP)
        return true;

    return false;
}

void robot_mvjoint(int joint, float val)
{
    if (robot_jointinv(joint))
    {
        val = val * -1.0f;
    }

    int mapped_val = robot_mapsrv(val, 200, 400);
    pwmWrite(PCA_9685_PIN_BASE + joint, mapped_val);
}

void robot_reset()
{
    for (int i = 0; i < SERVOS_NUM; i++)
    {
        servo[i] = 0.0f; 
    }
}


#endif