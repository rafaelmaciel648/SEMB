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
#include "uart.h"
#include "utils.h"

#define MAX_VEL 255


extern int *sensors_ptr;


void connectionLost(){
	return;
}

void task1(){
	printf("Iam in task 1\n");
	return;
}

void task2(){
	printf("Iam in task 2\n");
	return;
}

void robotMotion(){ ///CHANGE sensors_ptr[2] para outro sensor!!!!!!!!!!!!
	uint8_t vel;
	if( (sensors_ptr[1]) >=0 ){
		if(sensors_ptr[1] >= 85){
			vel = MAX_VEL;
		}
		else{
			vel = (sensors_ptr[1])*3;
		}

		// move forward M1
		leftMotorForward();
		// move forward M2
		rightMotorForward();

		leftVelocity(vel);
		rightVelocity(vel);

	}
	else{
		if(sensors_ptr[1] <= -85){
			vel = MAX_VEL;
		}
		else{
			vel = (-1) * (sensors_ptr[1]) * 3;
		}

		// move backward M1
		leftMotorBackward();
		// move backward M2
		rightMotorBackward();

		leftVelocity(vel);
		rightVelocity(vel);
	}

	printf("VELOCIDADE: %d\n",vel);
	//printf("%d %d %d\n",*(ypr),*(ypr+1),*(ypr+2));
}

void readData(){
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
//	static int sensores[3];

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
	sensors_ptr[0] = sensorA;
	sensors_ptr[1] = sensorB;
	sensors_ptr[2] = sensorC;
}


