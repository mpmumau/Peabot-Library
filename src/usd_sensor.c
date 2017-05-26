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
#include <string.h>

/* Raspberry Pi includes */
#include <wiringPi.h>

/* Application includes */
#include "main.h"
#include "config_defaults.h"
#include "utils.h"
#include "main.h"
#include "log.h"

/* Header */
#include "usd_sensor.h"

/* Forward decs */
static void *usd_sensor_main(void *arg);

static bool running = true;
static double distance;
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

double usd_sensor_getdist()
{
    return distance;
}

static void *usd_sensor_main(void *arg)
{
    struct timespec time;
    struct timespec last_time;

    double tick, diff, start_time, travel_time;

    clock_gettime(CLOCK_MONOTONIC, &last_time);
    while (running)
    {
        // Send the trigger signal for 20 microseconds
        digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, HIGH);
        while (tick < (20 * 0.000001))
        {
            clock_gettime(CLOCK_MONOTONIC, &time);
            diff = utils_timediff(time, last_time);
            last_time = time;
            tick += diff;            
        }
        digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, LOW);
        tick = 0.0;
        
        while(digitalRead(DEFAULT_HRC_SR04_ECHO_PIN) == LOW)
        {
            clock_gettime(CLOCK_MONOTONIC, &time);
            diff = utils_timediff(time, last_time);
            last_time = time;
            tick += diff;  

            if (tick > 0.5)
            {
                printf("timeout waiting for echo pin to start\n");
                break;
            }
        }
        tick = 0.0;

        clock_gettime(CLOCK_MONOTONIC, &time);
        start_time = utils_timespec_to_secs(time);
        while (digitalRead(DEFAULT_HRC_SR04_ECHO_PIN) == HIGH)
        {
            clock_gettime(CLOCK_MONOTONIC, &time);
            diff = utils_timediff(time, last_time);
            last_time = time;
            tick += diff;  

            if (tick > 0.5)
            {
                printf("timeout waiting for echo pin to stop\n");
                break;
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &time);
        travel_time = utils_timespec_to_secs(time) - start_time;

        tick = 0.0;

        distance = travel_time / 58.0;
        printf("distance: %f\n", distance);

        while (tick < 0.1)
        {
            clock_gettime(CLOCK_MONOTONIC, &time);
            diff = utils_timediff(time, last_time);
            last_time = time;
            tick += diff;              
        }

        tick = 0.0;
    }

    return (void *) NULL;
}

#endif