#include "stm32l476xx.h"

static void DAC_Pin_Init(void) {
    // configure PA4 as Analog, No Pull=Up, No Pull-Down
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable GPIOA clock
	
	GPIOB->MODER |= GPIO_MODER_MODER4; // set to Analog mode (11)
	
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4; // set to No Pull=Up, No Pull-Down (00)
}

void DAC_Write_Value(uint32_t value) {
    // [TODO] Write DAC1Ch1 right-aligned 12-bit value
}

void DAC_Init(void) {
    DAC_Pin_Init();

    // [TODO] Enable DAC clock

    // [TODO] Enable software trigger mode

    // [TODO] Disable trigger

    // [TODO] DAC1 connected in normal mode to external pin only with buffer enabled

    // [TODO] Enable DAC channel 1

    DAC_Write_Value(0);
}
