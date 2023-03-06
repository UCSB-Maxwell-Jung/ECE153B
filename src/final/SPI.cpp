#include "SPI.h"
#include <math.h>

SPI::SPI(SPI_TypeDef* SPIx)
	: _SPIx(SPIx) {}

void SPI::begin(uint32_t desired_freq) {
	_desired_freq = desired_freq;
	// calculate prescalar exponent (prescalar = 2^(n+1))
	_br = floor((log((float)CLK_FREQ/(float)_desired_freq)/log(2.0))-1);
	if (_br < 0)
		_br = 0;
	else if (_br > 7)
		_br = 7;
	_actual_freq = CLK_FREQ/(2<<(_br+1)); // divide by 2^(n+1)

	configure_GPIO();
	configure_SPI();
}

void SPI::enable(void) {
	_SPIx->CR1 |= SPI_CR1_SPE; // enable spi
}

void SPI::disable(void) {
	while ((_SPIx->SR & SPI_SR_FTLVL) != 0); // wait until no more data to transmit
	while ((_SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY); // wait until last data frame is processed
	_SPIx->CR1 &= ~SPI_CR1_SPE; // disable spi
	uint8_t discard;
	while ((_SPIx->SR & SPI_SR_FRLVL) != 0) // read all received data
		discard = *(volatile uint8_t*)(&_SPIx->DR); 
}

// send data out but don't care about data in
void SPI::transmit(uint8_t b) {
	// wait for Transmit Buffer Empty flag to be set
	while ((_SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE);
	// write byte to transfer
	*(volatile uint8_t*)(&_SPIx->DR) = b;
}

// transfer data out on output line and in on input line
uint8_t SPI::transmit_receive(uint8_t b) {
	// wait for Transmit Buffer Empty flag to be set
	while ((_SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE);
	// write byte to transfer
	*(volatile uint8_t*)(&_SPIx->DR) = b;

	// wait for data from slave
	while ((_SPIx->SR & SPI_SR_RXNE) != SPI_SR_RXNE);
	// read received byte
	return *(volatile uint8_t*)(&_SPIx->DR);
}

// initialize SPI2 GPIO pins
// void SPI::init_SPI2_GPIO(void) {
// 	uint32_t af_num;
// 	uint32_t pin_num;

// 	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //clk enabled
// 	// configure PB13, PB14, PB15

// 	// set to alternate function mode (10)
// 	// explanation: default is 11, so use mask &~(01) to set second bit to 0
// 	GPIOB->MODER &= ~(GPIO_MODER_MODER13_0 | 
// 					  GPIO_MODER_MODER14_0 | 
// 					  GPIO_MODER_MODER15_0);

// 	// reset AF for pin 13, 14, 15
// 	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL13 | 
// 	                   GPIO_AFRH_AFSEL14 | 
// 	                   GPIO_AFRH_AFSEL15);

// 	// set pin 13, 14, 15 to AF5
// 	af_num = 5;
// 	pin_num = 13;
// 	GPIOB->AFR[1] |= (af_num << ((pin_num%8)*4));
// 	pin_num = 14;
// 	GPIOB->AFR[1] |= (af_num << ((pin_num%8)*4));
// 	pin_num = 15;
// 	GPIOB->AFR[1] |= (af_num << ((pin_num%8)*4));

// 	// set to push-pull (0)
// 	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT13 | 
// 	                   GPIO_OTYPER_OT14 | 
// 	                   GPIO_OTYPER_OT15);

// 	// set to very high (11)
// 	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR13 | 
// 	                   GPIO_OSPEEDER_OSPEEDR14 | 
// 	                   GPIO_OSPEEDER_OSPEEDR15);
	
// 	// set to no pull-up, pull-down (00)
// 	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD13 | 
// 	                  GPIO_PUPDR_PUPD14 | 
// 	                  GPIO_PUPDR_PUPD15);
// }

// initialize SPI2 peripheral as slave
// void SPI::init_SPI2(void){
// 	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; // enable SPI2 clock
// 	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST; // set, then reset to clear SPI2
// 	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI2RST; 
// 	SPI2->CR1 &= ~SPI_CR1_SPE; // disable SPI2
// 	SPI2->CR1 &= ~SPI_CR1_RXONLY; // set to full-duplex (0)
// 	SPI2->CR1 &= ~SPI_CR1_BIDIMODE; // set to 2-line unidirectional data mode (0)
// 	SPI2->CR1 &= ~SPI_CR1_BIDIOE; // disable output in bidirectional mode (0)
// 	SPI2->CR1 &= ~SPI_CR1_LSBFIRST; // MSB first (0)
// 	SPI2->CR2 &= ~SPI_CR2_DS; // reset data size bits to 0000;
// 	SPI2->CR2 |= (SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0); // set data size to 8 (strangely, 8 corresponds to 0111 in the datasheet)
// 	SPI2->CR2 &= ~SPI_CR2_FRF; // use motorola SPI mode (0)
// 	SPI2->CR1 &= ~SPI_CR1_CPOL; // set clock to low polarity (0)
// 	SPI2->CR1 &= ~SPI_CR1_CPHA; // set clock to first clock transition (0)
// 	SPI2->CR1 &= ~SPI_CR1_BR; // reset baud rate control bits to 000
// 	SPI2->CR1 |= (SPI_CR1_BR_1 | SPI_CR1_BR_0); // set to 011 (prescalar value 16)
// 	SPI2->CR1 &= ~SPI_CR1_CRCEN; // disable hardware CRC calculation (0)
// 	SPI2->CR1 &= ~SPI_CR1_MSTR; // set to slave (0)
// 	SPI2->CR1 |= SPI_CR1_SSM; // enable SSM (software slave management) (1)
// 	SPI2->CR2 |= SPI_CR2_NSSP; // enable NSS pulse generation (1)
// 	SPI2->CR1 &= ~SPI_CR1_SSI; // set internal slave select bit (0)
// 	SPI2->CR2 |= SPI_CR2_FRXTH; // set FIFO threshold to 1/4 (1)
// 	SPI2->CR1 |= SPI_CR1_SPE; // enable SPI2
// }
