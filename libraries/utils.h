/*
 * utils.h
 *
 * Library with useful functions for the programming the robot.
 */

#ifndef LIBRARIES_UTILS_H_
#define LIBRARIES_UTILS_H_

void leftMotorForward();

void leftMotorBackward();

void rightMotorForward();

void rightMotorBackward();

void leftVelocity(uint8_t vel);

void rightVelocity(uint8_t vel);

void moveForward();

void moveBackward();

void stopRobot();

uint8_t int16ToString(int16_t number, uint8_t *buffer);

void clear_string(uint8_t *str);

#endif /* LIBRARIES_UTILS_H_ */
