#ifndef USD_SENSOR_DEF
#define USD_SENSOR_DEF

/*
 File:          usd_sensor.c
 Description:   Handler for the ultra-sonic distance sensor.
 Created:       May 24, 2017
 Author:        Matt Mumau
 */

#define _POSIX_C_SOURCE 199309L

/* System includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

/* Raspberry Pi includes */
#include <wiringPi.h>

/* Application includes */
#include "config_defaults.h"
#include "utils.h"
#include "main.h"
#include "log.h"

/* Header */
#include "usd_sensor.h"

/* Forward decs */
static void *usd_sensor_main(void *arg);

static bool running = true;
static float distance;
static pthread_t usd_thread;

void usd_sensor_init()
{
    int error = pthread_create(&usd_thread, NULL, usd_sensor_main, NULL);
    if (error)
        app_exit("[ERROR!] Could not initialize USD sensor thread.", 1);

    pinMode(DEFAULT_HRC_SR04_TRIGGER_PIN, OUTPUT);
    pinMode(DEFAULT_HRC_SR04_ECHO_PIN, INPUT);    

    digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, LOW);
}

void usd_sensor_halt()
{
    running = false;
    int error = pthread_join(usd_thread, NULL);
    if (error)
        log_event("[ERROR!] Could not rejoin from USD sensor thread.");
}

float usd_sensor_getdist()
{
    return distance;
}

static void *usd_sensor_main(void *arg)
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
    }

    return (void *) NULL;
}

#endif