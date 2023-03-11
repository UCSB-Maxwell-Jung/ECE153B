#include "I2C_Camera.h"

I2C_Camera::I2C_Camera()
    : I2C(I2C1) {}

// [TODO]
void I2C_Camera::configure_GPIO() {
	// RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //clk enabled
	// // configure PB6 for I2C1_SCL, PB7 for I2C1_SDA

	// // set both pins to alternate function mode (10)
	// // explanation: default is 11, so use mask &~(01) to set second bit to 0
	// GPIOB->MODER &= ~(GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);

	// // set pin 6,7 to AF4
	// int afWidth = 4;
	// GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7); // reset to AF0
	// GPIOB->AFR[0] |= (4 << (6*afWidth)); // pin 6 to AF4
	// GPIOB->AFR[0] |= (4 << (7*afWidth)); // pin 7 to AF4

	// // set both pins to open-drain (1)
	// GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);

	// // set both pins to very high (11)
	// GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
	
	// // set both pins to pull-up (01)
	// GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7);
	// GPIOB->PUPDR |= (GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0);
}

// [TODO]
void I2C_Camera::configure_I2C() {
	// uint32_t OwnAddr = 0x52;
	// //part B
	
	// RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; // 1.a enable peripheral clk

	// //set sys clk as src for I2C1 in the peripherals independent clock configuration register
	// RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL; 	//1.b 
	// RCC->CCIPR |=  RCC_CCIPR_I2C1SEL_0; 

	// //Reset I2C1 by setting bits in the peripheral reset register
	// RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST; //1.c
	// RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST; //clear bits 

	// //disable I2C
	// _I2Cx->CR1 &= ~I2C_CR1_PE;

	// //2.a
	// //enable error interrupts
	// _I2Cx->CR1 |= I2C_CR1_ERRIE;
	// //enable analog noise fileter, disable digital noise filter, enable clk stretching
	// _I2Cx->CR1 &= ~(I2C_CR1_ANFOFF | I2C_CR1_DNF | I2C_CR1_NOSTRETCH); 
	
	// //set master to operate in 7-bit addressing
	// _I2Cx->CR2 &= ~I2C_CR2_ADD10;
	// //enable auto-end mode, enable NACK generation
	// _I2Cx->CR2 |= (I2C_CR2_AUTOEND | I2C_CR2_NACK);

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

	// _I2Cx->TIMINGR |= ((7<<I2C_TIMINGR_PRESC_POS) 
	// 				 |(10<<I2C_TIMINGR_SCLDEL_POS)
	// 				 |(12<<I2C_TIMINGR_SDADEL_POS)
	// 				 |(47<<I2C_TIMINGR_SCLH_POS)
	// 				 |(40<<I2C_TIMINGR_SCLL_POS));
	
	// // part 3 disable own address
	// _I2Cx->OAR1 &= ~I2C_OAR1_OA1EN; 
	// _I2Cx->OAR2 &= ~I2C_OAR2_OA2EN;
	
	// //address set to 7 bit mode
	// _I2Cx->OAR1 &= ~I2C_OAR1_OA1MODE; //3.a
	
	// // Set Address to OwnAddr
	// _I2Cx->OAR1 |= OwnAddr; //3.b
	
	// // Enable Own1 Address
	// _I2Cx->OAR1 |= I2C_OAR1_OA1EN; //3.c
	
	// // Enable I2C1
	// _I2Cx->CR1 |= I2C_CR1_PE; //4
}
