#include "EXTI.h"

#include "DAC.h"
//Step 3.2 Interrupt .1
void EXTI_Init(void) {
    //CODE TAKEN FROM LAB2
  // Connect External Line to the GPIO
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	// Rising trigger selection
	// 0 = trigger disabled, 1 = trigger enabled
	EXTI->FTSR1 |= EXTI_RTSR1_RT13;

	// Interrupt Mask Register
	// 0 = masked, 1 = not masked (enabled)
	EXTI->IMR1 |= EXTI_IMR1_IM13;

	NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable
	NVIC_SetPriority(EXTI15_10_IRQn, 0); // set interrupt to highest priority
}

#define DAC_MIN 0
#define DAC_MAX 4095
#define DAC_INCREMENT 256

static uint32_t dac_value = 0;
static enum {
    DOWN,
    UP,
} direction = UP;

//step 3.2 .2
void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	if ((EXTI->PR1 & EXTI_PR1_PIF13) != 0) {
		// PC13 button is default HIGH for some reason
		dac_value = (DAC->DHR12R1); //read from reg
        if((dac_value < DAC_MAX) && (direction == UP))
        {
            dac_value += DAC_INCREMENT;
        }
        else if((dac_value >= DAC_MAX) && (direction == UP))
        {
            direction = DOWN;
        }
        else if(dac_value > DAC_MIN) && (direction == DOWN))
        {
            dac_value -= DAC_INCREMENT;
        }
        else if((dac_value <= DAC_MIN) && (direction == DOWN))
        {
            direction = UP;
        }else{
            //nothing...
        }
		// Cleared flag by writing 1
 		EXTI->PR1 |= EXTI_PR1_PIF13;
	}
}