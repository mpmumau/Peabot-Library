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

    double tick = 0.0;
    double diff = 0.0;

    unsigned int timeout, max_timeout;
    max_timeout = 10000000;

    FILE *logfile = NULL;
    char *filename = "/opt/peabot/usd.log";
    logfile = fopen(filename, "w");

    while (running)
    {
        clock_gettime(CLOCK_MONOTONIC, &time);
        diff = utils_timediff(time, last_time);
        last_time = time;
        
        tick += diff;

        digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, HIGH);
        delayMicroseconds(20);
        digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, LOW);

        while(digitalRead(DEFAULT_HRC_SR04_ECHO_PIN) == LOW)
        {
            timeout++;
            if (timeout == max_timeout)
                break;
        }
        timeout = 0;

        long startTime = micros();
        while(digitalRead(DEFAULT_HRC_SR04_ECHO_PIN) == HIGH)
        {
            timeout++;
            if (timeout == max_timeout)
                break;
        }
        long travelTime = micros() - startTime;
        timeout = 0;

        distance = travelTime / 58.0;

        delayMicroseconds(100000);

        fprintf(logfile, "%f,%f\n", tick, distance);
    }

    fclose(logfile);

    return (void *) NULL;
}

#endif