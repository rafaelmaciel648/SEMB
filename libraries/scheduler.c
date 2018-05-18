/*
 * scheduler.c
 *
 * Library with structures and functions for manage the schedule of task in system.
 *
 */


#include "scheduler.h"

#include <stdint.h>

uint8_t i;

Sched_Task_t Tasks[20];
int cur_task = 20;

int Sched_Init(void){
	/*- Initialise data structures.
	 *- Configure interrupt that periodically calls int_handler()*/
	return 0;
}

int Sched_AddT(void (*f)(void), int d, int p, int prio){
	if(Tasks[prio].func){
		return -1;
	}
	for(i=0; i<20; i++){
		if(!Tasks[i].func){
			Tasks[i].period = p;
			Tasks[i].delay = d;
			Tasks[i].exec = 0;
			Tasks[i].func = f;
			return i;
		}
	}
	return -1;
}

//void int_handler(void){
//	disable_interrupts();
//	Sched_Schedule();
//	Sched_Dispatch();
//	enable_interrupts();
//}

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
		//	enable_interrupts();
			Tasks[i].func();
			//disable_interrupts();
			cur_task = prev_task;
			/*Delete if one-shot */
			if (!Tasks[i].period)
				Tasks[i].func = 0;
			return;
		}
	}
};


