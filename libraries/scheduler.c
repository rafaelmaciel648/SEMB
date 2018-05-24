/*
 * scheduler.c
 *
 * Library with structures and functions for manage the schedule of task in system.
 *
 */


#include "scheduler.h"

#include <stdint.h>
#include <avr/interrupt.h>

uint8_t i;

Sched_Task_t Tasks[20];
int cur_task = 20;

#define	TICKS_TO_10MS		((uint16_t)(((double)F_CPU/8)/(100)))

int Sched_Init(void){
	/*- Initialise data structures.
	 *- Configure interrupt that periodically calls int_handler()*/
	ISR(TIMER0_OVF_vect)
	{
	  // reset timer0 count to give CTRL_FREQ frequency
	  TCNT0 = TCNT0 + (65536 - TICKS_TO_10MS);
	  int_handler();
	}
	// reset timer0 count
	TCNT1 = 0;
	// set timer1 prescaler to clk/8
	TCCR1B |= (1<<CS11);
	// clear pending interrupts
	TIFR1 |= (1 << TOV1);
	// enable timer1 overflow interrupt
	TIMSK1 |= (1 << TOIE1);

	return 0;
}

int Sched_AddT(void (*f)(void), int d, int p, int i){
	if(Tasks[i].func){
		return -1;
	}
	if(!Tasks[i].func){
		Tasks[i].period = p;
		Tasks[i].delay = d;
		Tasks[i].exec = 0;
		Tasks[i].func = f;
		return i;
	}
	return -1;
}

void int_handler(void){
	cli();						/* Disable interrupts */
	Sched_Schedule();
	Sched_Dispatch();
	sei();						/* Enable interrupts */
}

void Sched_Schedule(void){

	for(i=0; i<20; i++) {
		if (!Tasks[i].func)
			continue;

		if (Tasks[i].delay) {
			Tasks[i].delay--;
		} else {
			/* Schedule Task */
			Tasks[i].exec++;
			Tasks[i].delay = Tasks[i].period;
		}
	}
}

void Sched_Dispatch(void){
	int prev_task = cur_task;

	for(i=0; i<cur_task; i++){
		if (Tasks[i].exec) {
			Tasks[i].exec--;
			cur_task = i;
			sei();
			Tasks[i].func();
			cli();
			cur_task = prev_task;
			/*Delete if one-shot */
			if (!Tasks[i].period)
				Tasks[i].func = 0;
			return;
		}
	}
}


