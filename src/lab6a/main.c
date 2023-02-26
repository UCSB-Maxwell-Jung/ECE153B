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
	GPIOC->MODER &= ~(GPIO_MODER_MODE5 | 
					  GPIO_MODER_MODE6 |
					  GPIO_MODER_MODE8 |
					  GPIO_MODER_MODE9); // reset to 00
	GPIOC->MODER |= (GPIO_MODER_MODE5_0 | 
					 GPIO_MODER_MODE6_0 |
					 GPIO_MODER_MODE8_0 |
					 GPIO_MODER_MODE9_0); // set to output (01)

	// GPIO Output Speed: Low (00), Medium (01), High (10), Very High (11)
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5 |
						GPIO_OSPEEDR_OSPEED6 |
						GPIO_OSPEEDR_OSPEED8 |
						GPIO_OSPEEDR_OSPEED9); // reset to 00
	GPIOC->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5_1 | 
	                   GPIO_OSPEEDR_OSPEED6_1 |
					   GPIO_OSPEEDR_OSPEED8_1 |
					   GPIO_OSPEEDR_OSPEED9_1);  // set to High (10)
	
	GPIOC->OTYPER &- ~(GPIO_OTYPER_OT5 |
					   GPIO_OTYPER_OT6 |
					   GPIO_OTYPER_OT8 |
					   GPIO_OTYPER_OT9); // set output types to push-pull (0)

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5 | 
					  GPIO_PUPDR_PUPD6 |
					  GPIO_PUPDR_PUPD8 |
					  GPIO_PUPDR_PUPD9); // set to No Pull-Up, No Pull-Down (00)
}

// converts (2pi/8)n radians into 
// 4 bit value representing ABab
// where A = PC5, a = PC6, B = PC8, b = PC9
uint8_t angle_to_digital(uint8_t n) {
	uint8_t ABab;

	ABab = 1<<(n/2);
	if (n%2 == 1) // n is odd
		ABab += 1<<(((n+1)/2) % 4);

	return ABab;
}

#define A_MASK ((uint8_t)0x8U)
#define B_MASK ((uint8_t)0x4U)
#define a_MASK ((uint8_t)0x2U)
#define b_MASK ((uint8_t)0x1U)

#define A_output GPIO_ODR_OD5
#define a_output GPIO_ODR_OD6
#define B_output GPIO_ODR_OD8
#define b_output GPIO_ODR_OD9

// sets output of pins corresponding to ABab
// where A = PC5, a = PC6, B = PC8, b = PC9
// given digital signal ABab
void set_output(uint8_t ABab) {
	uint32_t value = 0; // set all pins as low by default
	if (ABab & A_MASK)
		value |= A_output; // set A (PC5) to high
	if (ABab & a_MASK)
		value |= a_output; // set a (PC6) to high
	if (ABab & B_MASK)
		value |= B_output; // set B (PC8) to high
	if (ABab & b_MASK)
		value |= b_output; // set b (PC9) to high

	GPIOC->ODR &= ~(A_output|a_output|B_output|b_output); // clear existing signals
	GPIOC->ODR |= value; // write new signal
}

#define DELAY 60000	// delay between steps of the sequences

void Full_Stepping_Clockwise_Sequence(void){
	uint8_t ABab;
	for (int i = 8; i > 0; i-=2) { // 8 (0), 6, 4, 2
		ABab = angle_to_digital(i%8);
		set_output(ABab);
		
		for (int delay = 0; delay < DELAY; delay++); // delay
	}
}

void Full_Stepping_CounterClockwise_Sequence(void){
	uint8_t ABab;
	for (int i = 0; i < 8; i+=2) { // 0, 2, 4, 6
		ABab = angle_to_digital(i&8);
		set_output(ABab);
		
		for (int delay = 0; delay < DELAY; delay++); // delay
	}
}

void Half_Stepping_Clockwise_Sequence(void){
	uint8_t ABab;
	for (int i = 8; i > 0; i-=2) { // 8, 7, 6, 5, 4, 3, 2, 1
		ABab = angle_to_digital(i%8);
		set_output(ABab);
		
		for (int delay = 0; delay < DELAY; delay++); // delay
	}
}

void Half_Stepping_CounterClockwise_Sequence(void){
	uint8_t ABab;
	for (int i = 0; i < 8; i+=1) { // 0, 1, 2, 3, 4, 5, 6, 7
		ABab = angle_to_digital(i&8);
		set_output(ABab);
		
		for (int delay = 0; delay < DELAY; delay++); // delay
	}
}

int main(void){
	GPIO_Init();

	uint32_t repeats = 512;

	// Rotate 360 degrees counter-clockwise using full-stepping
	for (int i = 0; i < repeats; i++) {
		Full_Stepping_CounterClockwise_Sequence();
	}

	// Rotate 360 degrees clockwise using half-stepping
	for (int i = 0; i < repeats; i++) {
		Half_Stepping_Clockwise_Sequence();
	}
}
