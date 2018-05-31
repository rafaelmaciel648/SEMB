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
#include "utils.h"


#define UART1_BAUD_RATE      57600	// baud rate for Bluetooth Communication


int16_t *sensors_ptr;					// Pointer to Sensor Data Array
int16_t distance;
uint8_t *str_lcd_l1;
uint8_t *str_lcd_l2;

uint8_t str1[] = "Embedded Systems";
uint8_t str2[] = "Press the button";
uint8_t str3[] = "Wait Connection ";
uint8_t str4[] = "Connection Lost ";
uint8_t str5[] = "Connecting...   ";

uint8_t lostconnection=0;


ISR(INT4_vect)
{
	lostconnection++;
	if(lostconnection > 16){			// Means 1us interrupt
		stopRobot();
		clear_lcd();
		print_lcd(str4,1,0);
		print_lcd(str5,2,0);
		while( !(PINE & (1<<PE4)) );
		lostconnection = 0;
	}
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

	int16_t ypr[3];
	ypr[0]=0;
	ypr[1]=0;
	ypr[2]=0;
	sensors_ptr=ypr;

	uint8_t line1[17] = "Embedded Systems";
	uint8_t line2[17];
	str_lcd_l1 = line1;
	str_lcd_l2 = line2;



	print_lcd(str1,1,0);

	//ROUTINE WAITING CONNECTION
	print_lcd(str3,2,0);
	while( !(PINE & (1<<PE4)) );
	EIMSK = (1<<INT4);

	//ROUTINE OF READY BUTTOM
	print_lcd(str2,2,0);
	while( !(PINC & (1<<PC1)) );

	Sched_Tasks();

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

