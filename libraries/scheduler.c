/*
 * scheduler.c
 *
 * Library with structures and functions for manage the schedule of task in system.
 *
 */


#include "scheduler.h"

#include <stdint.h>
#include <avr/interrupt.h>

#include "printf_tools.h"
#include "tasks.h"


uint8_t i;

Sched_Task_t Tasks[20];
int cur_task = 20;

#define	TICKS_TO_1MS		((uint16_t)(((double)F_CPU/8)/(1000)))

ISR(TIMER1_OVF_vect)
{
	// reset timer1 count to give CTRL_FREQ frequency
	TCNT1 = TCNT1 + (65536 - TICKS_TO_1MS);
	int_handler();
}

int Sched_Init(void){
	/*- Initialise data structures.
	 *- Configure interrupt that periodically calls int_handler()*/

	for(uint8_t i=0; i<20; i++){
		Tasks[i].func=NULL;
	}

	// reset timer1 count
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
		return 0;
	}
	if(!Tasks[i].func){
		Tasks[i].period = p;
		Tasks[i].delay = d;
		Tasks[i].exec = 0;
		Tasks[i].func = f;
		return i;
	}
	return 0;
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
			Tasks[i].exec=1;
			Tasks[i].delay = Tasks[i].period;
		}
	}
}

void Sched_Dispatch(void){
	int prev_task = cur_task;

	for(i=0; i<cur_task; i++){
		if (Tasks[i].exec) {
			Tasks[i].exec=0;
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

void Sched_Tasks(){
	Sched_AddT(sonarDistance, 0, 5, 1);
	Sched_AddT(readData, 0, 150, 2);
	Sched_AddT(robotMotion, 0, 5, 3);
	Sched_AddT(lcdRefresh, 0, 5, 4);
}

