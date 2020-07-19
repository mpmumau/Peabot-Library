/*
 File:          usd_sensor.h
 Description:   Handler for the ultra-sonic distance sensor.
 */

#ifndef USD_SENSOR_H_DEF
#define USD_SENSOR_H_DEF

/* HRC-SR04 config */
#define DEFAULT_HRC_SR04_ECHO_PIN 26
#define DEFAULT_HRC_SR04_TRIGGER_PIN 22

void usd_sensor_init();

void usd_sensor_halt();

double usd_sensor_getdist();

#endif