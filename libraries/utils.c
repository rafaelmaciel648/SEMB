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

void init_PWM(){

	/* TIMER 4 - PWM MOTOR 1 e 2 */
	DDRH |= (1<<DDH4)|(1<<DDH5);

	/* Set Timer4 to compare mode, set OC4A and OC4B on compare match, clear on TOP */
	TCCR4A = ( (1<<COM4B1)|(1<<COM4C1)|(1<<WGM40)  );
	/* Set Timer4 No Prescaling, Fast 8-bit PWM */
	TCCR4B = ( (1<<CS40)|(1<<WGM42) );

	OCR4B=0;
	OCR4C=0;
}

void configIO(){
	/* MOTOR OUTPUTS */
	DDRE |= (1<<DDE5);		// IN1 -> Motor 1 forward
	DDRG |= (1<<DDG5);		// IN2 -> Motor 1 backward
	DDRE |= (1<<DDE3);		// IN3 -> Motor 2 backward
	DDRH |= (1<<DDH3);		// IN4 -> Motor 2 forward

	/* PINS FOR OUTPUT TIME VALUES OF TASKS */
	DDRC |= (1<<DDC6);		// Output execution task readData()
	DDRC |= (1<<DDC4);		// Output execution task robotMotion()
	DDRC |= (1<<DDC2);		// Output execution task sonarDistance()
	DDRC |= (1<<DDC0);		// Output execution task lcdRefresh()

	/* BLUETOOTH STATUS */
//	DDRE &= ~(1<<DDE4);		//Bluetooth Esternal Interrupt Pin
//	EICRB = (0<<ISC41);		// Set external interrupt to check the communication status (INT4)
//	EIMSK = (1<<INT4);

	/* Sonar Pins */
	DDRL |= (1<<DDL3);		// TRIG output
	DDRL &= ~(1<<DDL1);		// ECHO input


	/*  */
}
