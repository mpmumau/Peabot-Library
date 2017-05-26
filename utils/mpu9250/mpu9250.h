#ifndef MPU_9250_H_DEF
#define MPU_9250_H_DEF

#define MPU_9250_ADDR 0x68

#define MPU_9250_REG_WHO_AM_I 0x75
#define MPU_9250_PWR_MGMNT_1 0x6B

#define MPU_9250_RESET 0x80


typedef struct MPU_9250 {
    int fd;
    uint8_t who_am_i;
} MPU_9250;

#endif