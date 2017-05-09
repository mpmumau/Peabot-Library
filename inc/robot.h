#ifndef ROBOT_H_DEF
#define ROBOT_H_DEF

/*
 File:          robot.h
 Description:   Peabot robot control functions declarations.
 Created:       May 7, 2017
 Author:        Matt Mumau
 */

struct ServoRange {
    int min;
    int max;
};

/* Initialize the robot device and its resources, and begin its loop. */
void robot_init();

/* Sets the min and max servo ranges for the pin. */
void robot_set_servo_limit(int pin, int min, int max);

/* Resets the robot to its "home" position */
void robot_reset();

/* Stop the robot, reset it and end its loop. */
void robot_halt();

/* Set the value of the servo at the given pin; will be mapped between -1.0 and 1.0 */
void robot_setservo(int pin, float val);

#endif