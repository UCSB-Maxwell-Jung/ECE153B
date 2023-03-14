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

void HardwareI2c1::configureI2c() {
	// [TODO]
}
