/*
 File:          robot.c
 Description:   Implementations of Peabot robot functions.
 */

#ifndef ROBOT_DEF
#define ROBOT_DEF

#define _POSIX_C_SOURCE 199309L

/* System includes */
#include <sys/prctl.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

/* Raspberry Pi libraries */
#include <wiringPi.h>
#include <pca9685.h>

/* Application includes */
#include "main.h"
#include "utils.h"

/* Header */
#include "robot.h"

/* Forward decs */
static void *robot_main(void *arg);
static void robot_mvjoint(unsigned short joint, double val);
static bool robot_jointinv(unsigned short joint);
static unsigned short robot_mapsrv(double val, ServoLimit *servo_limit);
static void robot_destroy();

static pthread_t    robot_thread;
static bool         running = true;
static int          error;
static int          pca_9685_fd;
static double       *servo;

void robot_init()
{
    unsigned int pca_9685_pin_base = DEFAULT_PCA_9685_PIN_BASE;
    unsigned int pca_9685_hertz = DEFAULT_PCA_9685_HERTZ;
    unsigned short servos_num = DEFAULT_SERVOS_NUM;

    pca_9685_fd = pca9685Setup(pca_9685_pin_base, 0x40, pca_9685_hertz);
    if (pca_9685_fd < 0)
        APP_ERROR("Could not get PCA-9685 file descriptor.", 1);
    pca9685PWMReset(pca_9685_fd);

    servo = calloc(servos_num, sizeof(double));
    if (!servo)
        APP_ERROR("Could not allocate memory.", 1);

    error = pthread_create(&robot_thread, NULL, robot_main, NULL);
    if (error)
        APP_ERROR("Could not initialize robot thread.", error);
}

void robot_halt()
{   
    robot_reset();
    robot_destroy();
    running = false;

    error = pthread_join(robot_thread, NULL);
    //if (error)
        //log_error("Could not rejoin from robot thread.", error);

    pca9685PWMReset(pca_9685_fd);
}

void robot_reset()
{
    if (!servo)
        return;

    unsigned short servos_num = DEFAULT_SERVOS_NUM;
    for (unsigned short i = 0; i < servos_num; i++)
        servo[i] = 0.0;
}

void robot_setservo(unsigned short pin, double val)
{
    servo[pin] = val;
}

double robot_getservo(unsigned short pin)
{
    return servo[pin];
}

static void *robot_main(void *arg)
{
    prctl(PR_SET_NAME, "PEABOT_ROBOT\0", NULL, NULL, NULL);

    struct timespec time;
    struct timespec last_time;

    double tick = 0.0;
    double diff;
    
    double robot_tick = DEFAULT_ROBOT_TICK;
    unsigned short servos_num = DEFAULT_SERVOS_NUM;

    clock_gettime(CLOCK_MONOTONIC, &last_time);

    while (running)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        diff = utils_timediff(time, last_time);
        last_time = time;
        
        tick += diff;
        if (tick < robot_tick)
            continue;

        tick = 0.0;

        for (unsigned short i = 0; i < servos_num; i++)
            robot_mvjoint(i, servo[i]); 
    }

    return (void *) NULL;
}

static void robot_mvjoint(unsigned short joint, double val)
{
    // TODO: Rewrite this.

    // val = robot_jointinv(joint) ? -val : val;

    // unsigned short pin_data = (unsigned short *) config_get(CONF_SERVO_PINS);
    // unsigned short pin = pin_data[joint];

    // ServoLimit *servo_limits = (ServoLimit *) config_get(CONF_SERVO_LIMITS);

    // unsigned short mapped_val = robot_mapsrv(val, &(servo_limits[joint]));

    // unsigned int *pca_9685_pin_base = (unsigned int *) config_get(CONF_PCA_9685_PIN_BASE);
    // pin += *pca_9685_pin_base;

    // pwmWrite(pin, mapped_val);
}

static bool robot_jointinv(unsigned short joint)
{
    if (joint == SERVO_INDEX_FRONT_LEFT_KNEE ||
        joint == SERVO_INDEX_FRONT_LEFT_HIP ||
        joint == SERVO_INDEX_BACK_RIGHT_KNEE ||
        joint == SERVO_INDEX_BACK_RIGHT_HIP)
        return true;

    return false;
}

static unsigned short robot_mapsrv(double val, ServoLimit *servo_limit)
{
    if (servo_limit->min >= servo_limit->max)
        return 0.0;

    if (val > 1.0)
        val = 1.0;
    if (val < -1.0)
        val = -1.0;

    double val_abs = fabs(val);
    double half = (servo_limit->max - servo_limit->min) / 2.0;
    double midway = (double) servo_limit->min + half;
    double diff = half * val_abs;

    if (val < 0.0)
        return (unsigned short) round(midway - diff);
    
    return (unsigned short) round(midway + diff);
}

static void robot_destroy()
{
    if (servo)
        free(servo);
    servo = NULL;
}

#endif