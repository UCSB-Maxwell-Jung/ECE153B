#include "ADC.h"

#include "stm32l476xx.h"

#include <stdint.h>

void ADC_Wakeup(void) {
    int wait_time;

    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
        ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power down mode if still in that state

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    // voltage regulator must first be enabled and the software must wait for the regulator start-up
    // time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator start-up time
    // The software must wait for the startup time of the ADC voltage regulator (T_ADCVREG_STUP)
    // before launching a calibration or enabling the ADC.
    // T_ADCVREG_STUP = 20 us
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
}

void ADC_Common_Configuration() {
    // [DONE] Part 2.1 
    // Enable Perpherial
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

     // step 1 enable I/O analog switch voltage booster
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;

    // step 2 enable 2.a V_REFINT
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;

    // Ensure that the clock is not divided. 2.b
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;

    // Ensure that the ADC clock scheme is set to HCLK/1 synchronous clock mode.
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE; //2.c
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	
    ADC123_COMMON->CCR &= ~ADC_CCR_DUAL; // 2.d
    /*
    All the ADCs independent:
    00000: Independent mode
    */
}

void ADC_Pin_Init(void) {
    // [DONE] Part 2.1 (pin setup)
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  // enable GPIOA

	// GPIO Mode: Input(00), Output (01),
	// AF(10), Analog (11) 
    GPIOA->PUPDR &= ~GPIO_MODER_MODE1;
	GPIOA->MODER |= GPIO_MODER_MODE1;// set to Analog

	// GPIO Push-Pull: No pull-up, pull-down (00),
	// Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1; // set to No Pull-Up, No Pull-Down

    //connect PA1 ro ADC input 
    GPIOA->ASCR |= GPIO_ASCR_ASC1;

}

void ADC_Init(void) {
    // [TODO] Enable & Reset ADC Clock
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN; //  .1 enable ADC clk
    
    RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST; // .2 reset ADC clk
    RCC->AHB2RSTR &= ~RCC_AHB2RSTR_ADCRST; //then clear reset bit

	ADC1->CR &= ~ADC_CR_ADEN; //.3 disable ADC

    // Other ADC Initialization
    ADC_Pin_Init();
    ADC_Common_Configuration();
    ADC_Wakeup();

    //.4 configure the ADC to have 12-bit resolution and rightalignment.
    ADC1->CFGR &= ~ADC_CFGR_ALIGN; //right alignment
    ADC1->CFGR &= ~ADC_CFGR_RES; //12bit resolution

    //.5
	ADC1->SQR1 &= ~ADC_SQR1_L; //set sequence length to 1
	ADC1->SQR1 |= (ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_2); //chan 6 used for 1st conversion
	
	//.6 single-ended Mode
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;
	
	//.7 
	ADC1->SMPR1 &= ~ADC_SMPR1_SMP6; //chan 6 cleared
	ADC1->SMPR1 |= (ADC_SMPR1_SMP6_0 | ADC_SMPR1_SMP6_1); //set to 24.5
	
	//.8 
	ADC1->CFGR &= ~ADC_CFGR_CONT; //single conversion mode 
	ADC1->CFGR &= ~ADC_CFGR_EXTEN; //hardware trigger detection is disabled
	
	//.9 Enable ADC and wait
	ADC1->CR |= ADC_CR_ADEN;
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
}
