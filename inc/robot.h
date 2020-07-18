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