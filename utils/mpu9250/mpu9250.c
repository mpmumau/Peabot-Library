#ifndef MPU_9250_DEF
#define MPU_9250_DEF


/* System includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Raspberry Pi includes */
#include <wiringPi.h>
#include <wiringPiI2C.h>

/* Header */
#include "mpu9250.h"

static MPU_9250 mpu_9250;

/* Forward decs */
static bool mpu_9250_write(uint8_t addr, uint8_t data);
static uint8_t mpu_9250_read(uint8_t addr);


static bool mpu_9250_write(uint8_t addr, uint8_t data)
{
    uint8_t buffer;
    buffer = data;

    wiringPiI2CWriteReg8(mpu_9250.fd, addr, buffer);
    delay(10);

    buffer = mpu_9250_read(addr);
    return buffer == data;
}

static uint8_t mpu_9250_read(uint8_t addr)
{
    uint8_t buffer;
    buffer = wiringPiI2CReadReg8(mpu_9250.fd, addr);
    return buffer;
}

int main(int argc, char *argv[])
{
    mpu_9250.fd = wiringPiI2CSetup(MPU_9250_ADDR);

    printf("Starting MPU-9250...\n");
    printf("MPU-9250 Linux file descriptor ID: %d\n", mpu_9250.fd);

    if (!mpu_9250_write(MPU_9250_PWR_MGMNT_1, MPU_9250_RESET))
        printf("Could not reset pwr.\n");
    delay(200);

    mpu_9250.who_am_i = (uint8_t) wiringPiI2CReadReg8(mpu_9250.fd, MPU_9250_REG_WHO_AM_I);
    printf("MPU-9250 - Who Am I: %d\n", mpu_9250.who_am_i);
}

#endif