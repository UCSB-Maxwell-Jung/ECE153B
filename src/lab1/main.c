/*
* ECE 153B - Winter 2023
*
* Name(s): Maxwell Jung, Baron Young
* Section: Wed 7pm
* Lab: 1A
*/
#include "stm32l476xx.h"
#include <stdbool.h>
enum button_state {pressed, released};
static enum button_state btn;

void init(void);
bool is_button_clicked(void);
void update_led(void);

void init(void){
	// Enable HSI
	RCC->CR |= ((uint32_t)RCC_CR_HSION);
	while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );

	// Select HSI as system clock source
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );

	// 2.2
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable Port A GPIO Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // Enable Port C GPIO Clock

	GPIOA->MODER &= ~(0x00000800UL); // 2.3a Set the mode of PA5 to Output
	GPIOA->OTYPER &= ~(0x00000020UL); // 2.3b Set the output type of PA 5 as Push-Pull
	GPIOA->PUPDR &= ~(0x00000c00UL); // 2.3c Set PA5 to No Pull-Up, No Pull-Down

	GPIOC->MODER &= ~(0x0c000000UL); // 2.4a Set mode of pin PC13 to Input
	GPIOC->PUPDR &= ~(0x0c000000UL); // 2.4b Set PC13 to no pull-up and no pull-down

	// Turn on Green LED
	GPIOA->ODR |= 1UL<<5;
	
	// set button starting state
	btn = released;
}

bool is_button_clicked(void){
	// PC13 button is default HIGH for some reason
	bool btn_pressed = ((GPIOC->IDR & (1UL<<13))>>13) == 0UL;
	bool btn_released = !btn_pressed;
	
	// state machine for detecting if button is going down or up 
	switch (btn){
		case released:
			if (btn_pressed)
				btn = pressed;
			break;
		case pressed:
			if (btn_released) {
				btn = released;
				return true;
			}
			break;
	}
	
	return false;
}

void update_led(void){
	GPIOA->ODR ^= 1UL<<5;
}

int main(void){
	// Initialization
	init();

	// Polling to Check for User Button Presses
	while(true) {
		if (is_button_clicked())
			update_led();
	}

	return 0;
}
