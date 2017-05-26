#ifndef USD_SENSOR_H_DEF
#define USD_SENSOR_H_DEF

/*
 File:          usd_sensor.h
 Description:   Handler for the ultra-sonic distance sensor.
 Created:       May 24, 2017
 Author:        Matt Mumau
 */

void usd_sensor_init();

void usd_sensor_halt();

double usd_sensor_getdist();

#endif