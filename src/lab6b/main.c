/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 6B
 */

#include "stm32l476xx.h"

// configure PA0
void SERVO_Pin_Init(){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable GPIOA clock

	GPIOA->MODER &= ~GPIO_MODER_MODE0; // reset moder to 00
	GPIOA->MODER |= GPIO_MODER_MODE0_1; // set moder to AF (10)

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL0; // reset to 0000
	GPIOA->AFR[0] |= 2; // set pin 0 to AF2 (TIM5_CH1)

	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0; // set to no pull-up, no pull-down (00)
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
