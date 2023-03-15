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

	// [TODO] configure timings
	// //2.a
	// //enable error interrupts
	// I2C1->CR1 |= I2C_CR1_ERRIE;	

	// //2.b
	// /*
	// Min Low clk per. = 4.7us
	// Min Hi clk per. = 4.0us
	// Min data setup time = 1000ns = 1us
	// Min data hold time = 1250ns = 1.25us
	
	// PRESC = 7
	// f_PRESC = 80MHz/(1+7) = 10MHz (t_PRESC = 0.1us)
	// SCLDEL = 0
	// t_SCLDEL = (SCLDEL + 1) * t_PRESC = (10 + 1)*0.1us = 1.1us Condition: [t_SCLDEL > 1us] 
	// SDADEL = 0
	// t_SDADEL = (SDADEL + 1) * t_PRESC = (12 + 1)*0.1us = 1.3us	     	 [t_SDADEL > 1.25us]
	// SCLL = 	0
	// t_SCLL = (SCLL + 1) * t_PRESC = (47 + 1)*0.1us = 4.8us		         [t_SCLL > 4.7us]
	// SCLH = 	0
	// t_SCLH = (SCLH + 1) * t_PRESC = (40 + 1)*0.1us = 4.1us		         [t_SCLH > 4.0us]

	
	// */

	// I2C1->TIMINGR |= ((7<<I2C_TIMINGR_PRESC_POS) 
	// 				 |(10<<I2C_TIMINGR_SCLDEL_POS)
	// 				 |(12<<I2C_TIMINGR_SDADEL_POS)
	// 				 |(47<<I2C_TIMINGR_SCLH_POS)
	// 				 |(40<<I2C_TIMINGR_SCLL_POS));

	I2Cx_->CR1 &= ~I2C_CR1_NOSTRETCH; // enable clock stretching

	I2Cx_->CR2 &= ~I2C_CR2_ADD10; // set to 7-bit addressing mode
}
