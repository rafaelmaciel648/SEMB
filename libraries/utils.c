/*
 * utils.c
 *
 * Library with useful functions for the programming the robot.
 */

#include <avr/io.h>
#include <string.h>

#include "printf_tools.h"

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

void moveForward(){
	// move forward M1
	leftMotorForward();
	// move forward M2
	rightMotorForward();
}

void moveBackward(){
	// move backward M1
	leftMotorBackward();
	// move backward M2
	rightMotorBackward();
}

void stopRobot(){
	// M1 stop
	leftMotorForward();
	leftMotorBackward();
	// M2 stop
	rightMotorForward();
	rightMotorBackward();
	V_LEFT = 0;
	V_RIGHT = 0;
}

uint8_t int16ToString(int16_t number, uint8_t *buffer){
	uint8_t length = 0;
	uint16_t n = number;
	uint16_t absNumber = number;

	if (number < 0)
	{
		buffer[length++] = '-';
		absNumber = -number;
	}
	n = absNumber;
	if (absNumber >= 10000)
	{
		buffer[length++] = n/10000 + '0';
		n = n % 10000;
	}
	if (absNumber >= 1000)
	{
		buffer[length++] = n/1000 + '0';
		n = n % 1000;
	}
	if (absNumber >= 100)
	{
		buffer[length++] = n/100 + '0';
		n = n % 100;
	}
	if (absNumber >= 10) buffer[length++] = n/10 + '0';

	buffer[length++] = (n % 10) + '0';

	buffer[length++]='\0';

	return length;
}

void clear_string(uint8_t *str){
	uint8_t i;
	for(i=0; i < 17; i++){
		str[i]='\0';
	}
}
