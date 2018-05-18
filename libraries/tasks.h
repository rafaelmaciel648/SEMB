/*
 * tasks.h
 *
 * Library with function that are specific task of the system. All functions must be (void) type.
 *
 */

#ifndef LIBRARIES_TASKS_H_
#define LIBRARIES_TASKS_H_

/*
 * Sporadic external interrupt, handled when bluetooth connection lost.
 * Execute robot stop routine and wait for connection
 */
void connectionLost();


/*
 * Periodic task to measure distance between robot and objects.
 * Execute robot stop routine if distance is short.
 */
void sonarDistance();


/*
 *
 */

#endif /* LIBRARIES_TASKS_H_ */
