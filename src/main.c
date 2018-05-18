/*
 *	Embedded Systems - FEUP
 *		 Title: Robot controlled by mobile Android sensors
 *  Created on: 2018
 *      Author: Rafael Maciel, Ricardo Ferreira, Wilson C�ndido
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <tasks.h>

#include "printf_tools.h"
#include "timer_tools.h"
#include "tasks.h"
#include "scheduler.h"
#include "utils.h"

#define	baud_monitor 57600u		// baud rate for serial monitor in computer
#define baud_ctrl 9600u			// baud rate for Bluetooth communication
// Caution: erroneous result if F_CPU is not correctly defined
#define baud_monitor_reg ((F_CPU/(16*baud_monitor))-1)  //baud divider
#define baud_ctrl_reg ((F_CPU/(16*baud_ctrl))-1)	//baud divider

void init_USART_MONITOR(void)
{
	/* Set baud value */
	UBRR0 = baud_monitor_reg;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void init_USART_CONTROLLER(void)
{
	/* Set baud value */
	UBRR1 = baud_ctrl_reg;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8data, 2stop bit */
	UCSR1C = (1<<USBS1)|(3<<UCSZ10);

	/* Set external interrupt to check the communication status (INT4) */
	EICRB = (3<<ISC00);

}

ISR(INT4_vect)
{
	printf("Interrupt detected at INT4\n");
}

int main(void)
{
	init_USART_MONITOR();			//init serial monitor communication
	init_USART_CONTROLLER();		//init serial bluetooth communication
	init_printf_tools();			//init printf tools
	sei();
	init_mili_timers();

	Sched_AddT(&connectionLost, 10, 10, 10);


	mili_timer T1, T3, T10, Tshow;           // Timers

	DDRA = (1<<PA1);
	PORTA = (1<<PA1);


	start_timer(&T1, 1000);   // Arrancar o timer com um tempo de 1000 milisegundos
	start_timer(&T3, 3000);
	start_timer(&T10,10000);
	start_timer(&Tshow, 100);


	while (1) {
		if(get_timer(&T1)){
			start_timer(&T1, 1000);   // Arrancar o timer com um tempo de 1000 milisegundos
		}

		if(get_timer(&T3)){
			start_timer(&T3, 3000);
		}

		if(get_timer(&T10)){
			start_timer(&T10, 10000);
		}

		if(get_timer(&Tshow)){
			start_timer(&Tshow, 100);
			printf("T1 = %d T3 = %d T10 = %d\r",
					get_timer_time(&T1), get_timer_time(&T3), get_timer_time(&T10));
		}
	}
}




//#include <avr/io.h>
//#include <util/delay.h>
//
//#define BLINK_DELAY_MS 1000
//
//int main (void)
//{
// /* set pin 5 of PORTB for output*/
// DDRE |= _BV(DDE4);
//
// while(1) {
//  /* set pin 5 high to turn led on */
//  PORTE |= _BV(PE4);
//  _delay_ms(BLINK_DELAY_MS);
//
//  /* set pin 5 low to turn led off */
//  PORTE &= ~_BV(PE4);
//  _delay_ms(BLINK_DELAY_MS);
// }
//}

