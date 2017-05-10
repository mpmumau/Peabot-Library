#ifndef ROBOT_H_DEF
#define ROBOT_H_DEF

/*
 File:          robot.h
 Description:   Peabot robot control functions declarations.
 Created:       May 7, 2017
 Author:        Matt Mumau
 */

typedef struct ServoLimit {
    int min;
    int max;
} ServoLimit;

/* Initialize the robot device and its resources, and begin its loop. */
void robot_init();

/* Stop the robot, reset it and end its loop. */
void robot_halt();

/* Resets the robot to its "home" position */
void robot_reset();

/* Sets the min and max servo ranges for the pin. */
void robot_set_servo_limit(int pin, int min, int max);

/* Set the value of the servo at the given pin; will be mapped between -1.0 and 1.0 */
void robot_setservo(int pin, float val);

/* Return the current value of a given servo. */
float robot_getservo(int pin);

#endif