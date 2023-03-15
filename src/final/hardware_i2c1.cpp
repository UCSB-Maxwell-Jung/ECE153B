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
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; // enable I2C1 clock
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST; // set, then reset to clear I2C1
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;

	I2Cx_->CR1 &= ~I2C_CR1_PE; // disable I2C1

	I2Cx_->CR1 &= ~I2C_CR1_ANFOFF; // enable analog noise filter
	I2Cx_->CR1 &= ~I2C_CR1_DNF; // disable digital noise filter

	// configure timings

	//set sys clk (80Mhz) as src clock for I2C1
	RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;
	RCC->CCIPR |=  RCC_CCIPR_I2C1SEL_0;

	/*
	Min data setup time = 100ns = 0.1us
	Min data hold time			= 0 us
	Min clk Low per.		 	= 1.3us
	Min clk Hi per.  	= 600ns = 0.6us
	
	Choose this prescalar value to make subsequent calculations easier:
	PRESC = 7
	f_PRESC = f_i2cclk/(1+PRESC) = 80MHz/(1+7) = 10MHz (t_PRESC = 0.1us)

	Find minimum possible delay values that fit [requirements]:
	SCLDEL = 1
	t_SCLDEL = (SCLDEL + 1) * t_PRESC = (1 + 1)*0.1us = 0.2us			[t_SCLDEL > 0.1us] 
	SDADEL = 0
	t_SDADEL = (SDADEL + 1) * t_PRESC = (0 + 1)*0.1us = 0.1us	     	[t_SDADEL > 0us]
	SCLL = 	13
	t_SCLL = (SCLL + 1) * t_PRESC = (13 + 1)*0.1us = 1.4us		        [t_SCLL > 1.3us]
	SCLH = 	6
	t_SCLH = (SCLH + 1) * t_PRESC = (6 + 1)*0.1us = 0.7us		        [t_SCLH > 0.6us]

	clock frequency must be below 400kHz, 
	meaning each clock cycle must be longer than 2.5us (1/400kHz).
	Period of each cycle consists of the time clock stays low + the time clock stays high
	or mathematically, t_clk = t_scll + t_sclh > 2.5us
	with above configuration, t_scll + t_sclh = 2.1us which is too fast
	so we need to add 0.4us of delay to either SCLL or SCLH

	I will decide to add to SCLH since it's the lower of the two.

	This brings the final delay values as:
	SCLDEL = 1  = 0.2us
	SDADEL = 0  = 0.1us
	SCLL   = 13 = 1.4us
	SCLH   = 6+4 = 1.1us

	Period of each clock is 1.4us + 1.1us = 2.5us = 400kHz

	In reality, the actual clock frequency will be even lower 
	because the above calculation excludes the time it takes to
	transition from LOW to HIGH (t_R) or HIGH to LOW (t_F).
	*/

	I2C1->TIMINGR |=  ((7<<I2C_TIMINGR_PRESC_POS) 
					  |(1<<I2C_TIMINGR_SCLDEL_POS)
					  |(0<<I2C_TIMINGR_SDADEL_POS)
					  |(10<<I2C_TIMINGR_SCLH_POS)
					  |(13<<I2C_TIMINGR_SCLL_POS));

	I2Cx_->CR1 &= ~I2C_CR1_NOSTRETCH; // enable clock stretching

	I2Cx_->CR2 &= ~I2C_CR2_ADD10; // set to 7-bit addressing mode
}
