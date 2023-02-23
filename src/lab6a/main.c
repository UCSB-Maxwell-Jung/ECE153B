/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 6A
 */

#include "stm32l476xx.h"

// initialize PC5, PC6, PC8, PC9
void GPIO_Init(void){	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // enable GPIOC
	
	// GPIO Mode: Input(00), Output (01), AF(10), Analog (11)
	GPIOA->MODER &= ~(GPIO_MODER_MODE5 | 
					  GPIO_MODER_MODE6 |
					  GPIO_MODER_MODE8 |
					  GPIO_MODER_MODE9); // reset to 00
	GPIOA->MODER |= (GPIO_MODER_MODE5_0 | 
					 GPIO_MODER_MODE6_0 |
					 GPIO_MODER_MODE8_0 |
					 GPIO_MODER_MODE9_0); // set to output (01)

	// GPIO Output Speed: Low (00), Medium (01), High (10), Very High (11)
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5 |
						GPIO_OSPEEDR_OSPEED6 |
						GPIO_OSPEEDR_OSPEED8 |
						GPIO_OSPEEDR_OSPEED9); // reset to 00
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5_1 | 
	                   GPIO_OSPEEDR_OSPEED6_1 |
					   GPIO_OSPEEDR_OSPEED8_1 |
					   GPIO_OSPEEDR_OSPEED9_1);  // set to High (10)
	
	GPIOA->OTYPER &- ~(GPIO_OTYPER_OT5 |
					   GPIO_OTYPER_OT6 |
					   GPIO_OTYPER_OT8 |
					   GPIO_OTYPER_OT9); // set output types to push-pull (0)

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5 | 
					  GPIO_PUPDR_PUPD6 |
					  GPIO_PUPDR_PUPD8 |
					  GPIO_PUPDR_PUPD9); // set to No Pull-Up, No Pull-Down (00)
}


#define DELAY 60000	// delay between steps of the sequences

void Full_Stepping_Clockwise(void){
	// [TODO]
}

void Full_Stepping_CounterClockwise(void){
	// [TODO]
}

void Half_Stepping_Clockwise(void){
	// [TODO]	
}

void Half_Stepping_CounterClockwise(void){
	// [TODO]
}


int main(void){
	GPIO_Init();
	
	// Rotate 360 degrees either clockwise or counter-clockwise
	// [TODO]
}
