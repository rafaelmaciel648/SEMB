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
#include "lcd.h"
#include "sonar.h"
//#include "utils.h"


#define UART1_BAUD_RATE      57600	// baud rate for Bluetooth Communication
#define EN_MOTOR1 0
#define EN_MOTOR2 0
#define M1_FORWARD 0
#define M1_BACK 0
#define M2_FORWARD 0
#define M2_BACK 0

int16_t *sensors_ptr;					// Pointer to Sensor Data Array
int16_t distance;
uint8_t *str_lcd_l1;
uint8_t *str_lcd_l2;


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

	DDRC |= (1<<DDC6);		// Output execution task readData()
	DDRC |= (1<<DDC4);		// Output execution task robotMotion()

	/* BLUETOOTH STATUS */
	//DDRE &= ~(1<<DDE4);		//Bluetooth Esternal Interrupt Pin
	//EICRB = (0<<ISC41);		// Set external interrupt to check the communication status (INT4)
	//EIMSK = (1<<INT4);

	/* Sonar Pins */
	DDRL |= (1<<DDL3);		// TRIG output
	DDRL &= ~(1<<DDL1);		// ECHO input


	/*  */
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
	init_lcd();
	init_sonar();

	uint8_t str1[] = "Embedded";
	uint8_t str2[] = "Systems";

	print_lcd(str1,1,0);
	print_lcd(str2,1,9);

	int16_t ypr[3];
	ypr[0]=0;
	ypr[1]=0;
	ypr[2]=0;

	sensors_ptr=ypr;

	uint8_t line1[17];
	uint8_t line2[17];
	str_lcd_l1 = line1;
	str_lcd_l2 = line2;

	//ROUTINE OF READY BUTTOM

	Sched_AddT(readData, 50, 15, 2);
	Sched_AddT(robotMotion, 55, 5, 3);
	Sched_AddT(sonarDistance, 45, 5, 1);
	Sched_AddT(lcdRefresh, 65, 10, 4);

	while(1){
		// infinite loop
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

