/*
 *	Embedded Systems - FEUP
 *		 Title: Robot controlled by mobile Android sensors
 *  Created on: 2018
 *      Author: Rafael Maciel, Ricardo Ferreira, Wilson C�ndido
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "printf_tools.h"
#include "uart.h"
#include "timer_tools.h"
#include "tasks.h"
#include "scheduler.h"
//#include "utils.h"


#define UART1_BAUD_RATE      9600	// baud rate for Bluetooth Communication
#define EN_MOTOR1 0
#define EN_MOTOR2 0
#define M1_FORWARD 0
#define M1_BACK 0
#define M2_FORWARD 0
#define M2_BACK 0

int *sensors_ptr;					// Pointer to Sensor Data Array

//ISR(INT4_vect)
//{
//	printf("Interrupt detected at INT4\n");
//
//}

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

	DDRE &= ~(1<<DDE4);

	/* BLUETOOTH STATUS */
	//EICRB = (0<<ISC41);	// Set external interrupt to check the communication status (INT4)
	//EIMSK = (1<<INT4);
}


int main(void)
{
	sei();													// Enable System Interrupts
	init_printf_tools();									// Initialise  Print to Monitor Tools
	init_mili_timers();										// Initialise Milliseconds Timers
	uart1_init( UART_BAUD_SELECT(UART1_BAUD_RATE,F_CPU) );	// Initialise Serial Bluetooth Communication
	init_PWM();
	configIO();
	Sched_Init();

	mili_timer readData_T, robotMotion_T;

	int ypr[3];
	ypr[0]=0;
	ypr[1]=0;
	ypr[2]=0;

	sensors_ptr=ypr;

//	if(!Sched_AddT(readData, 10, 10, 0)){
//		printf("Error adding task1\n");
//	}
//	if(!Sched_AddT(robotMotion, 17, 10, 1)){
//		printf("Error adding task2\n");
//	}

//	int *sensores;

	while(1){

		//start_timer(&readData_T, 500);		// Init timer for readData task
		readData();
		//printf("readData() task execution time: %u\n",get_timer_time(&readData_T));

		//start_timer(&robotMotion_T, 500);		// Init timer for robotMotion task
		robotMotion();
		//printf("robotMotion() task execution time: %u\n",get_timer_time(&robotMotion_T));

		//printf("%d - %d - %d || %d - %d - %d\n",ypr[0],ypr[1],ypr[2],sensors_ptr[0],sensors_ptr[1],sensors_ptr[2]);

	}
}

/*
 * NOTES:
 * DDRx -> Data Direction Register (0-input, 1-output)
 * PINx -> Read Input values
 * PORTx -> Write Output values
 *
 * Set as output:		DDRA |= (1<<DDA4);
 * Set as input:		DDRA &= ~(1<<DDA4);
 * Set pin to high:		PORTA |= (1<<PA4);
 * Set pin to low:		PORTA &= ~(1<<PA4);
 * Toogle pin:			PORTA ^= (1<<PA4);
 *
 */

