/*
 * tasks.c
 *
 * Library with function that are specific task of the system. All functions must be (void) type.
 *
 */

#include <tasks.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "printf_tools.h"
#include "uart.h"
#include "utils.h"
#include "timer_tools.h"
#include "sonar.h"
#include "lcd.h"


#define MAX_VEL 255
#define MAX_ROT 100


extern int *sensors_ptr;
extern uint16_t distance;
//extern uint8_t *str_lcd_l1;
extern uint8_t *str_lcd_l2;


void connectionLost(){
	return;
}



void robotMotion(){ ///CHANGE sensors_ptr[2] para outro sensor!!!!!!!!!!!!
	PORTC |= (1<<PC4);

	//printf("%d   %d\n",sensors_ptr[1], sensors_ptr[2]);

	uint8_t vel;
	uint8_t rot;

	if( (sensors_ptr[2]) >=0 ){						// MOVE FORWARD
		if(sensors_ptr[2] >= 85){
			vel = MAX_VEL;
		}
		else{
			vel = (sensors_ptr[2])*3;
		}

		moveBackward();

//		leftVelocity(vel);
//		rightVelocity(vel);

	}
	else{
		if(sensors_ptr[2] <= -85){					// MOVE BACKWARD
			vel = MAX_VEL;
		}
		else{
			vel = (-1) * (sensors_ptr[2]) * 3;
		}

		moveForward();

//		leftVelocity(vel);
//		rightVelocity(vel);
	}

	if( (sensors_ptr[1]) >=0 ){						// TURN LEFT
		if(sensors_ptr[1] >= 50){
			rot = MAX_ROT;
		}
		else{
			rot = (sensors_ptr[1])*2;
		}

		leftVelocity(vel);
		rightVelocity( vel - (vel*rot)/100 );

	}
	else{
		if(sensors_ptr[1] <= -50){					// TURN RIGHT
			rot = MAX_ROT;
		}
		else{
			rot = (-1) * (sensors_ptr[1]) * 2;
		}

		leftVelocity( vel - (vel*rot)/100 );
		rightVelocity(vel);
	}

	PORTC &= ~(1<<PC4);
}



void readData(){
	PORTC |= (1<<PC6);

	uint8_t done = 0;
	char a;
	uint8_t read = 0;
	uint8_t comma=0;
	int16_t sensorA = 0;
	int16_t sensorB = 0;
	int16_t sensorC = 0;
	uint8_t minusSensA = 0;
	uint8_t minusSensB = 0;
	uint8_t minusSensC = 0;
	uint8_t sensorAisDone = 0;
	uint8_t sensorBisDone = 0;
	uint8_t sensorCisDone = 0;

	while(!done){
		a = uart1_getc();

		if(a == '\n'){
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
			sensorA=0;
			sensorB=0;
			sensorC=0;
			comma = 0;
			continue;
		}
		if(a == ','){ //finds a comma, which means a new value comes and allows to know which sensor should be filled.

			comma++;
			continue;
		}

		//-----------------------------sensA

		if(read && comma == 2 && a=='-'){ //SensorA will be a negative number.
			minusSensA = 1;
			continue;

		}
		if(read && comma == 2 && (! sensorAisDone) && (a>47) && (a<58)){ //its time to read, its after the 1st comma, and its a number: so save it for the 1st sensor
			sensorA = (sensorA*10 + a) - 48;
			continue;
			/*if(a== '0'){
				sensorA = sensorA*10;
			}
			else{
				sensorA = (sensorA*10 + a) - 48;
			}*/

		}
		if(read && comma == 2 && (a == '.')){
			sensorAisDone = 1;
			continue;
		}


		//------------------sensB
		if(read && comma == 3 && a=='-'){ //SensorB will be a negative number.
			minusSensB = 1;
			continue;

		}
		if(read && comma == 3 && (! sensorBisDone) && (a>47) && (a<58)){ //its time to read, its after the 1st comma, and its a number: so save it for the 1st sensor
			sensorB = (sensorB*10 + a) - 48;
			continue;
		}
		if(read && comma == 3 && (a == '.')){
			sensorBisDone = 1;
			continue;
		}

		//------------------sensC
		if(read && comma == 4 && a=='-'){ //SensorC will be a negative number.
			minusSensC = 1;
			continue;

		}
		if(read && comma == 4 && (! sensorCisDone) && (a>47) && (a<58)){ //its time to read, its after the 1st comma, and its a number: so save it for the 1st sensor

			sensorC = (sensorC*10 + a) - 48;
			continue;
		}
		if(read && comma == 4 && (a == '.')){
			sensorCisDone = 1;
			continue;
		}
	}

	if(minusSensA) sensorA *= (-1);
	if(minusSensB) sensorB *= (-1);
	if(minusSensC) sensorC *= (-1);

	sensors_ptr[0] = sensorA;
	sensors_ptr[1] = sensorB;
	sensors_ptr[2] = sensorC;

	PORTC &= ~(1<<PC6);
}


void sonarDistance(){
	measure_distance();

	//printf("Distanceinsonar: %d          ",distance);

	if(distance < 10){
		stopRobot();
	}
}

void lcdRefresh(){
	uint8_t str_aux[6]={'\0','\0','\0','\0','\0','\0'};

	int16ToString(distance,str_aux);			// Convert int to string

	str_lcd_l2[0] = 0;							// Clear string to be printed

	strcat((char*)str_lcd_l2,"Distance: ");
	strcat((char*)str_lcd_l2,(char*)str_aux);
	strcat((char*)str_lcd_l2," cm");
	strcat((char*)str_lcd_l2,"\0");

	print_lcd(str_lcd_l2,2,0);
}


//void readData2(){
//	//start_timer(&t3,500);
//	char a;
//	uint8_t minusA=0, minusB=0, minusC=0;
//	int16_t yaw;
//	int16_t pitch;
//	int16_t roll;
//
//	yaw=0; pitch=0; roll=0;
//
//	// Loop until find '>'
//	do{
//		a = uart1_getc();
//	} while(a != '>');
//
//	// Loop until find first ','
//	do{
//		a = uart1_getc();
//	} while(a != ',');
//
//	// Loop until find second ','
//	do{
//		a = uart1_getc();
//	} while(a != ',');
//
//	// Check if YAW negative
//	a = uart1_getc();
//	if(a=='-'){
//		minusA=1;
//		a = uart1_getc();
//	}
//
//	// Read YAW sensor
//	do{
//		yaw = (yaw*10) + (a-48);
//		a = uart1_getc();
//	} while(a != '.');
//
//	// Find next ','
//	do{
//		a = uart1_getc();
//	} while(a != ',');
//
//	// Check if PITCH negative
//	a = uart1_getc();
//	if(a=='-'){
//		minusB=1;
//		a = uart1_getc();
//	}
//
//	// Read PITCH sensor
//	do{
//		pitch = (pitch*10) + (a-48);
//		a = uart1_getc();
//	} while(a != '.');
//
//	// Find next ','
//	do{
//		a = uart1_getc();
//	} while(a != ',');
//
//	// Check if ROLL negative
//	a = uart1_getc();
//	if(a=='-'){
//		minusC=1;
//		a = uart1_getc();
//	}
//
//	// Read ROLL sensor
//	do{
//		roll = (roll*10) + (a-48);
//		a = uart1_getc();
//	} while(a != '.');
//
//	if(minusA) yaw *= (-1);
//	if(minusB) pitch *= (-1);
//	if(minusC) roll *= (-1);
//
//	sensors_ptr[0] = yaw;
//	sensors_ptr[1] = pitch;
//	sensors_ptr[2] = roll;
//
//	//printf("time READ: %d  |||||||||||",get_timer_time(&t3));
//	printf("%d %d %d\n", yaw, pitch, roll);
//}
//


//void readData3(){
//	PORTC |= (1<<PC6);
//	uint8_t i;
//	char read[100];
//	char a;
//
//	uint8_t minusA=0, minusB=0, minusC=0;
//	int16_t yaw;
//	int16_t pitch;
//	int16_t roll;
//
//	do{
//		a=uart1_getc();
//	}while(a!='>');
//
//	for(i=0; a != '\n'; i++){
//		read[i]=a;
//		a=uart1_getc();
//	}
//	read[i]='\0';
//
//	//printf("string: %s\n",read);
//
//	PORTC &= ~(1<<PC6);
//
//
//	yaw=0; pitch=0; roll=0;
//	i=0;
//
//	// Loop until find '>'
//	do{
//		a = read[i];
//	} while(a != '>');
//
//	// Loop until find first ','
//	do{
//		a = read[i++];
//	} while(a != ',');
//
//	// Loop until find second ','
//	do{
//		a = read[i++];
//	} while(a != ',');
//
//	// Check if YAW negative
//	a = read[i++];
//	if(a=='-'){
//		minusA=1;
//		a = read[i++];
//	}
//
//	// Read YAW sensor
//	do{
//		yaw = (yaw*10) + (a-48);
//		a = read[i++];
//	} while(a != '.');
//
//	// Find next ','
//	do{
//		a = read[i++];
//	} while(a != ',');
//
//	// Check if PITCH negative
//	a = read[i++];
//	if(a=='-'){
//		minusB=1;
//		a = read[i++];
//	}
//
//	// Read PITCH sensor
//	do{
//		pitch = (pitch*10) + (a-48);
//		a = read[i++];
//	} while(a != '.');
//
//	// Find next ','
//	do{
//		a = read[i++];
//	} while(a != ',');
//
//	// Check if ROLL negative
//	a = read[i++];
//	if(a=='-'){
//		minusC=1;
//		a = read[i++];
//	}
//
//	// Read ROLL sensor
//	do{
//		roll = (roll*10) + (a-48);
//		a = read[i++];
//	} while(a != '.');
//
//	if(minusA) yaw *= (-1);
//	if(minusB) pitch *= (-1);
//	if(minusC) roll *= (-1);
//
//	sensors_ptr[0] = yaw;
//	sensors_ptr[1] = pitch;
//	sensors_ptr[2] = roll;
//
//	printf("          %d %d %d\n", yaw, pitch, roll);
//}
