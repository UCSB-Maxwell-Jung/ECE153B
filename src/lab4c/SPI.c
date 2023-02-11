#include "SPI.h"

// Note: When the data frame size is 8 bit, "SPIx->DR = byte_data;" works incorrectly. 
// It mistakenly send two bytes out because SPIx->DR has 16 bits. To solve the program,
// we should use "*((volatile uint8_t*)&SPIx->DR) = byte_data";

void SPI2_GPIO_Init(void) {
	// initialize SPI2 GPIO pins

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //clk enabled
	// configure PB3, PB4, PB5, PB13, PB14, PB15

	// set to alternate function mode (10)
	// explanation: default is 11, so use mask &~(01) to set second bit to 0
	GPIOB->MODER &= ~(GPIO_MODER_MODER3_0 | \
					  GPIO_MODER_MODER4_0 | \
					  GPIO_MODER_MODER5_0 | \
					  GPIO_MODER_MODER13_0 | \
					  GPIO_MODER_MODER14_0 | \
					  GPIO_MODER_MODER15_0);

	//set to AF//
	// reset AF for pin 3, 4, 5, 13, 14, 15
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL3 | \
	                   GPIO_AFRL_AFSEL4 | \
	                   GPIO_AFRL_AFSEL5);
	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL13 | \
	                   GPIO_AFRH_AFSEL14 | \
	                   GPIO_AFRH_AFSEL15);

	// set pin 3, 4, 5, 13, 14, 15 to AF5
	int af_num = 5;
	int pin_num;
	pin_num = 3;
	GPIOB->AFR[0] |= (af_num << (pin_num*4));
	pin_num = 4;
	GPIOB->AFR[0] |= (af_num << (pin_num*4));
	pin_num = 5;
	GPIOB->AFR[0] |= (af_num << (pin_num*4));
	pin_num = 13;
	GPIOB->AFR[1] |= (af_num << ((pin_num%8)*4));
	pin_num = 14;
	GPIOB->AFR[1] |= (af_num << ((pin_num%8)*4));
	pin_num = 15;
	GPIOB->AFR[1] |= (af_num << ((pin_num%8)*4));

	// set to push-pull (0)
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT3 | \
	                   GPIO_OTYPER_OT4 | \
	                   GPIO_OTYPER_OT5 | \
	                   GPIO_OTYPER_OT13 | \
	                   GPIO_OTYPER_OT14 | \
	                   GPIO_OTYPER_OT15);

	// set to very high (11)
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR3 | \
	                   GPIO_OSPEEDER_OSPEEDR4 | \
	                   GPIO_OSPEEDER_OSPEEDR5 | \
	                   GPIO_OSPEEDER_OSPEEDR13 | \
	                   GPIO_OSPEEDER_OSPEEDR14 | \
	                   GPIO_OSPEEDER_OSPEEDR15);
	
	// set to no pull-up, pull-down (00)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD3 | \
	                  GPIO_PUPDR_PUPD4 | \
	                  GPIO_PUPDR_PUPD5 | \
	                  GPIO_PUPDR_PUPD13 | \
	                  GPIO_PUPDR_PUPD14 | \
	                  GPIO_PUPDR_PUPD15);
}

void SPI2_Init(void){
	// TODO: initialize SPI2 peripheral
}
 
void SPI_Transfer_Byte(SPI_TypeDef* SPIx, uint8_t write_data, uint8_t* read_data) {
	// TODO: perform SPI transfer
}
