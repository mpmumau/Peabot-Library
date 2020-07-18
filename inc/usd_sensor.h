/*
 File:          usd_sensor.h
 Description:   Handler for the ultra-sonic distance sensor.
 */

#ifndef USD_SENSOR_H_DEF
#define USD_SENSOR_H_DEF

void usd_sensor_init();

void usd_sensor_halt();

double usd_sensor_getdist();

#endif