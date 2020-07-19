/*
 File:          robot.h
 Description:   Peabot robot control functions declarations.
 */

#ifndef ROBOT_H_DEF
#define ROBOT_H_DEF

typedef struct ServoLimit {
    unsigned short min;
    unsigned short max;
} ServoLimit;

#define DEFAULT_PCA_9685_PIN_BASE 300
#define DEFAULT_PCA_9685_MAX_PWM 4096
#define DEFAULT_PCA_9685_HERTZ 50

#define DEFAULT_SERVOS_NUM 8
#define DEFAULT_ROBOT_TICK 0.01

/* PCA_9685 servo pins; left/right relative to the robot. */
#define DEFAULT_BACK_LEFT_KNEE 0
#define DEFAULT_BACK_LEFT_HIP 1
#define DEFAULT_FRONT_LEFT_KNEE 3
#define DEFAULT_FRONT_LEFT_HIP 2
#define DEFAULT_BACK_RIGHT_KNEE 4
#define DEFAULT_BACK_RIGHT_HIP 5
#define DEFAULT_FRONT_RIGHT_KNEE 6
#define DEFAULT_FRONT_RIGHT_HIP 7

/* Enums for working with servos */
#define SERVO_INDEX_BACK_LEFT_KNEE 0
#define SERVO_INDEX_BACK_LEFT_HIP 1
#define SERVO_INDEX_FRONT_LEFT_KNEE 2
#define SERVO_INDEX_FRONT_LEFT_HIP 3
#define SERVO_INDEX_BACK_RIGHT_KNEE 4
#define SERVO_INDEX_BACK_RIGHT_HIP 5
#define SERVO_INDEX_FRONT_RIGHT_KNEE 6
#define SERVO_INDEX_FRONT_RIGHT_HIP 7

/* Initialize the robot device and its resources, and begin its loop. */
void robot_init();

/* Stop the robot, reset it and end its loop. */
void robot_halt();

/* Resets the robot to its "home" position */
void robot_reset();

/* Set the value of the servo at the given pin; will be mapped between -1.0 and 1.0 */
void robot_setservo(unsigned short pin, double val);

/* Return the current value of a given servo. */
double robot_getservo(unsigned short pin);

#endif