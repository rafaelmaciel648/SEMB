/*
 * scheduler.h
 *
 * Library with structures and functions for manage the schedule of task in system.
 *
 */

#ifndef LIBRARIES_SCHEDULER_H_
#define LIBRARIES_SCHEDULER_H_

typedef struct {
	/* priority 0->HIGH 18->LOW */
	int priority;
	/* period in ticks */
	int period;
	/* ticks to activate */
	int delay;
	/* function pointer */
	void (*func)(void);
	/* activation counter */
	int exec;
} Sched_Task_t;



/* Initialise data structures.
 * Configure interrupt that periodically calls int_handler()
 */
int Sched_Init(void);


/* Add a task to the schedule
 * *f - pointer to the task function
 * d - delay of execution
 * p - period
 * i - priority
 */
int Sched_AddT(void (*f)(void), int d, int p, int i);


/* Verifies if any task needs to be activated, and if so,
 * increments by 1 the task's pending activation counter.
 */
void Sched_Schedule(void);


/* Verifies if any task, with higher priority than currently executing task,
 * has an activation counter > 0, and if so, calls that task.
 */
void Sched_Dispatch(void);


/* int_handler is called periodicly with interrupt timer 1.
 * Calls periodicly Sched_Schedule() and Sched_Dispatch().
 */
void int_handler(void);


/*
 *
 */
void Sched_Tasks();

#endif /* LIBRARIES_SCHEDULER_H_ */
