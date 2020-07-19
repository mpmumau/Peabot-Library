/*
 File:          usd_sensor.c
 Description:   Handler for the ultra-sonic distance sensor.
 */

#ifndef USD_SENSOR_DEF
#define USD_SENSOR_DEF

#define _POSIX_C_SOURCE 199309L

/* System includes */
#include <sys/prctl.h>
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
#include "utils.h"
#include "main.h"

/* Header */
#include "usd_sensor.h"

/* Forward decs */
static void *usd_sensor_main(void *arg);

static double distance;
static pthread_t usd_thread;
static int error;

void usd_sensor_init()
{
    pinMode(DEFAULT_HRC_SR04_TRIGGER_PIN, OUTPUT);
    pinMode(DEFAULT_HRC_SR04_ECHO_PIN, INPUT);

    digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, LOW);
}

double usd_sensor_getdist()
{
    return distance;
}

static void *usd_sensor_main(void *arg)
{
    double new_distance;

    unsigned int timeout, max_timeout;
    max_timeout = 1000000;

    bool timeout_error = false;

    timeout = 0;
    timeout_error = false;

    digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(DEFAULT_HRC_SR04_TRIGGER_PIN, LOW);

    while (digitalRead(DEFAULT_HRC_SR04_ECHO_PIN) == LOW && !timeout_error)
    {
        timeout++;
        if (timeout > max_timeout)
            timeout_error = true;
    }
    timeout = 0;

    //Wait for echo end
    long start_time = micros();
    while (digitalRead(DEFAULT_HRC_SR04_ECHO_PIN) == HIGH && !timeout_error)
    {
        timeout++;
        if (timeout > max_timeout)
            timeout_error = true;
    }
    long travel_time = micros() - start_time;

    //Get distance in cm
    if (!timeout_error)
    {
        new_distance = travel_time / 58.0;
        if (new_distance <= 400.0)
        {
            distance = new_distance;
        }
    }

    return (void *) NULL;
}

#endif