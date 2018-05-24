/*
 *	Embedded Systems - FEUP
 *		 Title: Robot controlled by mobile Android sensors
 *  Created on: 2018
 *      Author: Rafael Maciel, Ricardo Ferreira, Wilson C�ndido
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "printf_tools.h"
#include "uart.h"
#include "timer_tools.h"
#include "tasks.h"
#include "scheduler.h"
//#include "utils.h"


#define UART1_BAUD_RATE      9600	// baud rate for Bluetooth Communication
#define EN_MOTOR1 0
#define EN_MOTOR2 0
#define M1_FORWARD 0
#define M1_BACK 0
#define M2_FORWARD 0
#define M2_BACK 0

static int *sensors_ptr;

ISR(INT4_vect)
{
	printf("Interrupt detected at INT4\n");

}

void init_PWM(){

	/* TIMER 4 - PWM MOTOR 1 e 2 */
	DDRH |= (1<<DDH4)|(1<<DDH5);

	/* Set Timer4 to compare mode, set OC4A and OC4B on compare match, clear on TOP */
	TCCR4A = ( (1<<COM4B1)|(1<<COM4C1)|(1<<WGM40)  );
	/* Set Timer4 No Prescaling, Fast 8-bit PWM */
	TCCR4B = ( (1<<CS40)|(1<<WGM42) );

	OCR4B=0;
	OCR4C=0;
}

void configIO(){
	/* MOTOR OUTPUTS */
	DDRE |= (1<<DDE5);		// IN1 -> Motor 1 forward
	DDRG |= (1<<DDG5);		// IN2 -> Motor 1 backward
	DDRE |= (1<<DDE3);		// IN3 -> Motor 2 backward
	DDRH |= (1<<DDH3);		// IN4 -> Motor 2 forward

	DDRE &= ~(1<<DDE4);

	/* BLUETOOTH STATUS */
	EICRB = (0<<ISC41);	// Set external interrupt to check the communication status (INT4)
	EIMSK = (1<<INT4);
}

int* readData();

int main(void)
{
	sei();													// Enable System Interrupts
	init_printf_tools();									// Initialise  Print to Monitor Tools
	init_mili_timers();										// Initialise Milliseconds Timers
	uart1_init( UART_BAUD_SELECT(UART1_BAUD_RATE,F_CPU) );	// Initialise Serial Bluetooth Communication
	init_PWM();
	configIO();

	int ypr[3];

	sensors_ptr=ypr;

//	Sched_AddT(readData())
//	int *sensores;

	while(1){

//		sensores = readData();
//		robotMotion(sensores);

	}
}

/*
 * NOTES:
 * DDRx -> Data Direction Register (0-input, 1-output)
 * PINx -> Read Input values
 * PORTx -> Write Output values
 *
 * Set as output:		DDRA |= (1<<DDA4);
 * Set as input:		DDRA &= ~(1<<DDA4);
 * Set pin to high:		PORTA |= (1<<PA4)  ou  PORTA |= _BV(PA4);
 * Set pin to low:		PORTA &= ~(1<<PA4)  ou  PORTE &= ~_BV(PE4);
 * Toogle pin:			PORTA ^= (1<<PA4)
 *
 */


int* readData(){
	int i = 0;
	int done = 0;
	char a;
	int read = 0;
	int comma=0;
	int sensorA = 0;
	int sensorB = 0;
	int sensorC = 0;
	int minusSensA = 0;
	int minusSensB = 0;
	int minusSensC = 0;
	int sensorAisDone = 0;
	int sensorBisDone = 0;
	int sensorCisDone = 0;
	static int sensores[3];

	while(!done){
		a = uart1_getc();

		if(a == '\n'){
			if (minusSensC){ //if if it ends reading sensorC, see if it need negative signal. then after the number is full read, multiplies itc with -1; after, reset the minusSensC.

				sensorC = sensorC * (-1);
				minusSensC = 0;
			}
			break;
		}

		if(a == '>'){ //waits for the initial character and sets 'read=true'

			read= 1;
			minusSensA = 0;
			minusSensB = 0;
			minusSensC = 0;
			sensorAisDone = 0;
			sensorBisDone = 0;
			sensorCisDone = 0;
			comma = 0;
		}
		if(a == ','){ //finds a comma, which means a new value comes and allows to know which sensor should be filled.

			comma++;

		}

		//-----------------------------sensA

		if(read && comma == 2 && a=='-'){ //SensorA will be a negative number.
			minusSensA = 1;

		}
		if(read && comma == 2 && (! sensorAisDone) && (a>47) && (a<58)){ //its time to read, its after the 1st comma, and its a number: so save it for the 1st sensor
			sensorA = (sensorA*10 + a) - 48;
			/*if(a== '0'){
				sensorA = sensorA*10;
			}
			else{
				sensorA = (sensorA*10 + a) - 48;
			}*/

		}
		if(read && comma == 2 && (a == '.')){
			sensorAisDone = 1;
		}
		if (minusSensA && (comma > 2)){ //if it starts reading sensorB and caught a minus signal for sensA, then after the number is full read, multiplies it with -1; after, reset the minusSensA.
			//printf("\n multiplicacaos !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			sensorA = sensorA * (-1);
			minusSensA = 0;
		}

		//------------------sensB
		if(read && comma == 3 && a=='-'){ //SensorB will be a negative number.
			minusSensB = 1;

		}
		if(read && comma == 3 && (! sensorBisDone) && (a>47) && (a<58)){ //its time to read, its after the 1st comma, and its a number: so save it for the 1st sensor
			sensorB = (sensorB*10 + a) - 48;
		}
		if(read && comma == 3 && (a == '.')){
			sensorBisDone = 1;
		}
		if (minusSensB && (comma > 3)){ //if it starts reading sensorC and caught a minus signal for sensA, then after the number is full read, multiplies it with -1; after, reset the minusSensB.

			sensorB = sensorB * (-1);
			minusSensB = 0;
		}

		//------------------sensC
		if(read && comma == 4 && a=='-'){ //SensorC will be a negative number.
			minusSensC = 1;

		}
		if(read && comma == 4 && (! sensorCisDone) && (a>47) && (a<58)){ //its time to read, its after the 1st comma, and its a number: so save it for the 1st sensor

			sensorC = (sensorC*10 + a) - 48;
		}
		if(read && comma == 4 && (a == '.')){
			sensorCisDone = 1;


		}

		i++;
	}
	sensores[0] = sensorA;
	sensores[1] = sensorB;
	sensores[2] = sensorC;
	return sensores;
}



