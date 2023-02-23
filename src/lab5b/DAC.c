#include "stm32l476xx.h"

static void DAC_Pin_Init(void) {
    // configure PA4 as Analog, No Pull=Up, No Pull-Down
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable GPIOA clock
	
	GPIOB->MODER |= GPIO_MODER_MODER4; // set to Analog mode (11)
	
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4; // set to No Pull=Up, No Pull-Down (00)

}

void DAC_Write_Value(uint32_t value) {
    // [TODO] Write DAC1Ch1 right-aligned 12-bit value
    DAC->DHR12R1 = value;
}

void DAC_Init(void) {
    DAC_Pin_Init();
    //Step 3.1
    // [DONE] Enable DAC clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //.1

    DAC->CR &= ~(DAC_CR_EN1); //.2

    // [DONE] Enable software trigger mode
    DAC->CR &= ~(DAC_CR_TSEL1);
    DAC->CR |= DAC_CR_TSEL1; //.3
    
    // [DONE] Disable trigger
    DAC->CR &= ~(DAC_CR_TEN1); //.4

    // [DONE] DAC1 connected in normal mode to external pin only with buffer enabled
    DAC->MCR &= ~(DAC_MCR_MODE1);

    // [DONE] Enable DAC channel 1
    DAC->CR |= DAC_CR_EN1; //.5

    DAC_Write_Value(0);
}
