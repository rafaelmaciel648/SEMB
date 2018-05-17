
#include "scheduler.h"

#include <stdint.h>

uint8_t i;

int Sched_Init(void){
	/*- Initialise data structures.
	 *- Configure interrupt that periodically calls int_handler()*/
	return 0;
}

int Sched_AddT(void (*f)(void), int d, int p){
	for(int x=0; x<20; x++){
		if(!Tasks[x].func){
			Tasks[x].period = p;
			Tasks[x].delay = d;
			Tasks[x].exec = 0;
			Tasks[x].func = f;
			return x;
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

	for(int x=0; x<cur_task; x++){
		if (Tasks[x].exec) {
			Tasks[x].exec--;
			cur_task = x;
		//	enable_interrupts();
			Tasks[x].func();
			//disable_interrupts();
			cur_task = prev_task;
			/*Delete if one-shot */
			if (!Tasks[x].period)
				Tasks[x].func = 0;
			return;
		}
	}
};


