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

    bool initial_delay = true;
    while (initial_delay)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        diff = utils_timediff(time, last_time);
        last_time = time;
        
        tick += diff;

        printf("diff: %f | tick: %f\n", diff, tick);

        if (tick < 1.0f)
            continue; 

        tick = 0.0f;
        diff = 0.0f;
        initial_delay = false;   
    }

        printf("Starting USD Sensor main.\n");

    bool is_transmit = true;
    bool transmit_on = false;
    bool waiting_echo = true;
    float transmit_time = 0.1f;

    while (running)
    {
        printf("USD running\n");
        clock_gettime(CLOCK_MONOTONIC, &time);
        diff = utils_timediff(time, last_time);
        last_time = time;
        
        tick += diff;

        if (is_transmit)
        {
            if (tick < transmit_time)
            {
                if (transmit_on == false)
                {
                    digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, HIGH);
                    transmit_on = true;
                }
                continue;
            }

            digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, LOW);
            transmit_on = false;

            tick = 0.0f;
            is_transmit = false;
        }
        else
        {
            if (waiting_echo)
            {
                if (digitalRead(DEFAULT_HRC_SR04_ECHO_PIN) == LOW)
                    continue;

                waiting_echo = false;
                tick = 0.0f;
            }

            if (digitalRead(DEFAULT_HRC_SR04_ECHO_PIN) == HIGH)
                continue;

            waiting_echo = true;

            distance = (tick * 0.000000001) / 58;

            printf("USD Distance: %f\n", distance);

            is_transmit = true;
        }
    }

    return (void *) NULL;
}

#endif