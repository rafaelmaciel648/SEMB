/*
 * sonar.c
 *
 *  Created on: 25 May 2018
 *      Author: rafae
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "printf_tools.h"

//#define	SONAR_TICKS		((uint8_t)(((double)F_CPU)/(100000)))
#define TRIG_PIN PL3

extern uint16_t distance;

uint8_t timer5_overflow_count;
uint16_t rising, falling;
uint32_t counts;
uint32_t time_us;

ISR(TIMER5_OVF_vect)
{
	timer5_overflow_count++;
}

ISR (TIMER5_CAPT_vect)
{
	/*  Input Capture Unit
	 */
	if (TCCR5B & (1<<ICES5)) 			// On rising edge
	{
	  TCNT5 = 0;
	  timer5_overflow_count = 0;
	  TCCR5B &= ~(1<<ICES5); 			// Next time detect falling edge
	  rising = ICR5; 					// Save current count

	}
	else 								// On falling edge
	{
		TCCR5B |= (1<<ICES5);			// Next time detect rising edge
		falling = ICR5; 				// Save current count

		time_us = ( (uint32_t)falling + ( 65535 * timer5_overflow_count) ) * 0.5;
		distance = ( time_us * 0.034 ) / 2;
	}
}

void init_sonar(){
	// reset timer5 count
	TCNT5 = 0;
	// Set all bit to zero Normal operation
	TCCR5A = 0;
	// set timer5 prescaler to clk/8, rising edge trigger
	TCCR5B |= (1<<CS51) | (1<<ICES5);
	// clear pending interrupts
	TIFR5 |= (1 << TOV5);
	// enable timer5 overflow interrupt, input capture interrupt
	TIMSK5 |= (1 << TOIE5) | (1<<ICIE5);


}

void measure_distance(){

	/* Give 10us trigger pulse on trig. pin to HC-SR04 */
	PORTL &= (~(1 << TRIG_PIN));
	_delay_us(2);
	PORTL |= (1 << TRIG_PIN);
	_delay_us(15);
	PORTL &= (~(1 << TRIG_PIN));

}


//
//ISR(TIMER5_OVF_vect)
//{
//	timer5_overflow_count++;
//}
//
////ISR (TIMER4_CAPT_vect)
////{
////  /*  Input Capture Unit
////   */
////  if (TCCR4B & (1<<ICES4)) // On rising edge
////  {
////    TCCR4B &= ~(1<<ICES1); // Next time detect falling edge
////    rising = ICR1; // Save current count
////  }
////  else // On falling edge
////  {
////    TCCR4B |= (1<<ICES4); // Next time detect falling edge
////    falling = ICR4; // Save current count
////    counts = (uint32_t)falling - (uint32_t)rising;
////    distance = ( ( 0.5 * counts ) * 0.034 ) / 2; // useconds / 58 to get distance in cm
////  }
////}
//
//void init_sonar(){
//	// reset timer5 count
//	TCNT5 = 0;
//	// Set all bit to zero Normal operation
//	TCCR5A = 0;
//	// set timer5 prescaler to clk/8, rising edge trigger
////	TCCR5B |= (1<<CS51) | (1<<ICES5);
//	TCCR5B |= (1<<CS51);
//	// clear pending interrupts
//	TIFR5 |= (1 << TOV5);
//	// enable timer5 overflow interrupt, input capture interrupt
////	TIMSK5 |= (1 << TOIE5) | (1<<ICIE5);
//	TIMSK5 |= (1 << TOIE5);
//
//
//}
//
//int measure_distance(){
//	uint16_t distance;
//	uint32_t time_us;
//
//	/* Give 10us trigger pulse on trig. pin to HC-SR04 */
//	PORTL |= (1 << TRIG_PIN);
//	_delay_us(15);
//	PORTL &= (~(1 << TRIG_PIN));
//
//	TCNT5 = 0;						/* Clear Timer counter */
//	TCCR5B = (1<<ICES5)|(1<<CS51);	/* Capture on rising edge */
//	TIFR5 = 1<<ICF5;				/* Clear ICP flag (Input Capture flag) */
//	TIFR5 = 1<<TOV5;				/* Clear Timer Overflow flag */
//
//	/*Calculate width of Echo by Input Capture (ICP) */
//
//	while ((TIFR5 & (1 << ICF5)) == 0);	/* Wait for rising edge */
//	TCNT5 = 0;							/* Clear Timer counter */
//	TCCR5B = (0<<ICES5)|(1<<CS51);		/* Capture on falling edge, No prescaler */
//	TIFR5 = 1<<ICF5;					/* Clear ICP flag (Input Capture flag) */
//	TIFR5 = 1<<TOV5;					/* Clear Timer Overflow flag */
//	timer5_overflow_count = 0;
//
//	while ((TIFR5 & (1 << ICF5)) == 0);	/* Wait for falling edge */
//
//	/* 16MHz Timer Freq, Sound speed = 340 m/s */
//	time_us = ( ICR5 + (65535 * timer5_overflow_count) )* 0.5;
//	distance = (time_us * 0.034) / 2;
//	printf("2- Time: %ld | Distance: %u\n",time_us,distance);
//
//	return distance;
//}


