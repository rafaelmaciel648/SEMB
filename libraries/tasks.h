/*
 * tasks.h
 *
 * Library with function that are specific task of the system. All functions must be (void) type.
 *
 */

#ifndef LIBRARIES_TASKS_H_
#define LIBRARIES_TASKS_H_

extern int *sensors_ptr;

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
 * Read data from Bluetooh Serial Connection.
 * Right the values of Yaw, Pitch and Roll sensors in ypr[] array.
 */
void readData();


/*
 *	Set the motion of robot. Direction and Velocity.
 */
void robotMotion();


void lcdRefresh();


#endif /* LIBRARIES_TASKS_H_ */
