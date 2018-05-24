/*
 * utils.c
 *
 * Library with useful functions for the programming the robot.
 */

#include <avr/io.h>

#define V_RIGHT OCR4C
#define V_LEFT OCR4B

void leftMotorForward(){
	PORTE |= (1<<PE5);
	PORTG &= ~(1<<PG5);
}

void leftMotorBackward(){
	PORTE &= ~(1<<PE5);
	PORTG |= (1<<PG5);
}

void rightMotorForward(){
	PORTH |= (1<<PH3);
	PORTE &= ~(1<<PE3);
}

void rightMotorBackward(){
	PORTH &= ~(1<<PH3);
	PORTE |= (1<<PE3);
}

void leftVelocity(uint8_t vel){
	V_LEFT = vel;
}

void rightVelocity(uint8_t vel){
	V_RIGHT = vel;
}
