#ifndef MPU_9250_DEF
#define MPU_9250_DEF


/* System includes */
#include <stdio.h>

/* Raspberry Pi includes */
#include <wiringPi.h>
#include <wiringPiI2C.h>

/* Header */
#include "mpu9250.h"

static MPU_9250 mpu_9250;

int main(int argc, char *argv[])
{
    mpu_9250.fd = wiringPiI2CSetup(MPU_9250_ADDR);

    printf("Starting MPU-9250...\n");
    printf("MPU-9250 Linux file descriptor ID: %d\n", mpu_9250.fd);

    mpu_9250.who_am_i = (uint8_t) wiringPiI2CReadReg8(mpu_9250.fd, MPU_9250_REG_WHO_AM_I);
    printf("MPU-9250 - Who Am I: %d\n", mpu_9250.who_am_i);
}

#endif