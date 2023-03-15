#include "hardware_i2c1.h"

HardwareI2c1::HardwareI2c1()
    : I2C(I2C1) {}

// configure PB6 for I2C1_SCL, PB7 for I2C1_SDA
void HardwareI2c1::configureGpio() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIOB

	// set both pins to alternate function mode (10)
	GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7); // reset to 00
	GPIOB->MODER |=  (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1); // set to 10

	// set pin 6,7 to AF4
	uint8_t af_num = 4;
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7); // reset to AF0
	GPIOB->AFR[0] |= (af_num << (6*4)); // PB6 to AF4
	GPIOB->AFR[0] |= (af_num << (7*4)); // PB7 to AF4

	// set both pins to open-drain (1)
	GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);

	// set both pin output speeds to very high (11)
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
	
	// set both pins to pull-up (01)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7);
	GPIOB->PUPDR |=  (GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0);
}

// initialize I2C following the flowchart on Figure 351 (pg 1107) of reference manual
void HardwareI2c1::configureI2c() {
	 //slave is 0x60 for write 0x61 for read
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; // enable I2C1 clock
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST; // set, then reset to clear I2C1
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;

	I2Cx_->CR1 &= ~I2C_CR1_PE; // disable I2C1

	I2Cx_->CR1 &= ~I2C_CR1_ANFOFF; // enable analog noise filter
	I2Cx_->CR1 &= ~I2C_CR1_DNF; // disable digital noise filter

	// [TODO] configure timings
	/*
	PPRE1[2:0]:APB low-speed prescaler (APB1) generated from HCLK or
	Set and cleared by software to control the division factor of the APB1 clock (PCLK1).
	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16
	*/
	RCC->CFGR &= ~(RCC_CFGR_PPRE1);
	RCC->CFGR |=  (RCC_CFGR_PPRE1_2 | RCC_CFGR_PPRE1_2);  //HCLK divided by 8
	//not sure if should be done here, but it needs to be set

	// Min Low clk per. = 1.3us
	// Min Hi clk per. = 600ns
	// Min data setup time = 100ns 
	// Min data hold time = 0us

	//CPU clk (HCLK) = 80MHz/16= 5Mhz
	//(f= 5Mhz then period of i2c clk) = t_i2cclk = 0.2us
	
	//t_PRESC = (PRESC+1) x t_I2CCLK = (12+1)* 0.2us = 2.6us
	// PRESC = 12
	// f_PRESC = 5MHz/(12+1) = 0.384MHz = 384KHz (t_PRESC = 2.6us)
	// SCLDEL = 0
	// t_SCLDEL = (SCLDEL + 1) * t_PRESC = (0 + 1)*2.6us = 2.6us  Condition: [t_SCLDEL(min data setup) > 1us] 
	// SDADEL = 0
	// t_SDADEL = (SDADEL + 1) * t_PRESC = (0 + 1)*2.6us = 2.6us	     	 [t_SDADEL(min data hold) > 1.25us]
	// SCLL = 	1
	// t_SCLL = (SCLL + 1) * t_PRESC = (1 + 1)*2.6us = 5.2us		         [t_SCLL(min low clk per.) > 4.7us]
	// SCLH = 	1
	// t_SCLH = (SCLH + 1) * t_PRESC = (1 + 1)*2.6us = 5.2us		         [t_SCLH(min hi clk per.) > 4.0us]

	I2C1->TIMINGR |= ((12<<I2C_TIMINGR_PRESC_POS) 
					 |(0<<I2C_TIMINGR_SCLDEL_POS)
					 |(0<<I2C_TIMINGR_SDADEL_POS)
					 |(1<<I2C_TIMINGR_SCLH_POS)
					 |(1<<I2C_TIMINGR_SCLL_POS));
}
