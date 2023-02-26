/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 6B
 */

#include "stm32l476xx.h"

void SERVO_Pin_Init(){
	// [TODO]
}

void TIM5_CH1_Init(){
	// [TODO]
}


void Servo_Move_Left_90_Degree(){
	// function to move the servo to +90 degrees
	// [TODO]
}

void Servo_Move_Center(){
	// function to move the servo to 0 degrees
	// [TODO] 
}

void Servo_Move_Right_90_Degree(){
	// function to move the servo to -90 degrees
	// [TODO]
}


int main() {
	int i;

	SERVO_Pin_Init();
	TIM5_CH1_Init();
		
    while(1) {
			// Move servo to +90 degrees, 0 degrees, and -90 degrees
			// [TODO]
			
			for(i=0;i<1000;i++);  		// delay
    }
}
