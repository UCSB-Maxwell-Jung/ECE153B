#include "EXTI.h"

#include "DAC.h"
// Step 3.2 Interrupt .1
// CODE TAKEN FROM LAB2
void EXTI_Init(void) {
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
#define DAC_DELTA 256

static int next_dac_value = 0;
static enum {DOWN, UP} direction = UP;

//step 3.2 .2
void EXTI15_10_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13) {
		next_dac_value = (DAC->DHR12R1); // read from reg

        if (direction == UP) {
            next_dac_value += DAC_DELTA;
            if (next_dac_value > DAC_MAX) {
                next_dac_value = DAC_MAX;
                direction = DOWN;
            }
        }
        else if (direction == DOWN) {
            next_dac_value -= DAC_DELTA;
            if (next_dac_value < DAC_MIN) {
                next_dac_value = DAC_MIN;
                direction = UP;
            }
        }
        
        DAC_Write_Value(next_dac_value); 
		// Cleared flag by writing 1
 		EXTI->PR1 |= EXTI_PR1_PIF13;
	}
}
