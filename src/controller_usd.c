#ifndef CONTROLLER_USD_DEF
#define CONTROLLER_USD_DEF

/*
 File:          controller_usd.c
 Description:   Controller functions for dealing with USD model data.
 Created:       May 31, 2017
 Author:        Matt Mumau
 */

/* System includes */
#include <stdio.h>
#include <stdbool.h>

/* Libraries */
#include "cJSON.h"

/* Application includes */
#include "usd_sensor.h"
#include "mvc_data.h"

/* Header */
#include "controller_usd.h"

bool cntlusd_getval(MVCData *mvc_data)
{   
    double distance = usd_sensor_getdist();
    cJSON_AddNumberToObject(mvc_data->response_json, "distance" , distance);
    return true;
}

#endif