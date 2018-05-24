/*
 * tasks.c
 *
 * Library with function that are specific task of the system. All functions must be (void) type.
 *
 */

#include <tasks.h>
#include <avr/io.h>
#include <util/delay.h>

#include "printf_tools.h"

#define V_M1 OCR4C
#define V_M2 OCR4B



void connectionLost(){
	return;
}

void robotMotion(int *ypr){
	uint8_t vel;
	if( *(ypr+1) >=0 ){
		vel = *(ypr+1)*3;

		// move forward M1
		PORTE |= (1<<PE5);
		PORTG &= ~(1<<PG5);
		// move forward M2
		PORTH |= (1<<PH3);
		PORTE &= ~(1<<PE3);

		V_M1 = vel;
		V_M2 = vel;

	}
	else{
		vel = (-1) * (*(ypr+1)) * 3;

		// move forward M1
		PORTE &= ~(1<<PE5);
		PORTG |= (1<<PG5);
		// move forward M2
		PORTH &= ~(1<<PH3);
		PORTE |= (1<<PE3);

		V_M1 = vel;
		V_M2 = vel;
	}

	printf("VELOCIDADE: %d\n",vel);
	//printf("%d %d %d\n",*(ypr),*(ypr+1),*(ypr+2));
}


