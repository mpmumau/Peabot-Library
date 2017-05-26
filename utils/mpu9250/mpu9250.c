#ifndef MPU_9250_DEF
#define MPU_9250_DEF


/* System includes */
#include <stdio.h>

/* Raspberry Pi includes */
#include <wiringPi.h>
#include <wiringPiI2C.h>

/* Header */
#include "mpu9250.h"

int main(int argc, char *argv[])
{
    int setup = wiringPiI2CSetup(MPU_9250_ADDR);

    printf("Setup: %d\n", setup);
}

#endif