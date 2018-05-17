/*
 * scheduler.h
 *
 *  Created on: 8 May 2018
 *      Author: rafae
 */

#ifndef LIBRARIES_SCHEDULER_H_
#define LIBRARIES_SCHEDULER_H_

typedef struct {
	/* period in ticks */
	int period;
	/* ticks to activate */
	int delay;
	/* function pointer */
	void (*func)(void);
	/* activation counter */
	int exec;
} Sched_Task_t;

Sched_Task_t Tasks[20];
int cur_task = 20;


/* Initialise data structures.
 * Configure interrupt that periodically calls int_handler()
 */
int Sched_Init(void);


/* Add a task to the schedule
 * *f - pointer to the task function
 * d - delay of execution
 * p - period
 */
int Sched_AddT(void (*f)(void), int d, int p);


/* Verifies if any task needs to be activated, and if so,
 * increments by 1 the task's pending activation counter.
 */
void Sched_Schedule(void);


/* Verifies if any task, with higher priority than currently executing task,
 * has an activation counter > 0, and if so, calls that task.
 */
void Sched_Dispatch(void);

#endif /* LIBRARIES_SCHEDULER_H_ */
