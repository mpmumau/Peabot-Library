#ifndef MPU_9250_H_DEF
#define MPU_9250_H_DEF

#define MPU_9250_ADDR 0x68

#define MPU_9250_REG_WHO_AM_I 0x75
#define MPU_9250_PWR_MGMNT_1 0x6B
#define MPU_9250_PWR_MGMNT_2 0x6C
#define MPU_9250_I2C_SLV0_ADDR 0x25
#define MPU_9250_I2C_SLV0_REG 0x26
#define MPU_9250_I2C_SLV0_CTRL 0x27
#define MPU_9250_EXT_SENS_DATA_00 0x49

#define MPU_9250_RESET 0x80
#define MPU_9250_CLOCK_SEL_PLL 0x01
#define MPU_9250_SENSORS_ENABLE 0x00

#define MPU_9250_I2C_READ_FLAG 0x80
#define MPU_9250_I2C_SLV0_EN 0x80

#define AK8963_I2C_ADDR 0x0C
#define MPU_9250_AK8963_WHO_AM_I 0x00



typedef struct MPU_9250 {
    int fd;
    uint8_t who_am_i;
    uint8_t ak8963_who_am_i;
} MPU_9250;

#endif