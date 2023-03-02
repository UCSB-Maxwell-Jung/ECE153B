#include "SPI.h"
#include <math.h>

// Note: When the data frame size is 8 bit, "SPIx->DR = byte_data;" works incorrectly. 
// It mistakenly send two bytes out because SPIx->DR has 16 bits. To solve the program,
// we should use "*((volatile uint8_t*)&SPIx->DR) = byte_data";

SPI::SPI(SPI_TypeDef* SPIx) : _SPIx(SPIx) {}

void SPI::begin(uint32_t freq) {
	if (_SPIx == SPI1) {
		init_SPI1_GPIO();
		init_SPI1(freq);
	}
	else if (_SPIx == SPI2) {
		init_SPI2_GPIO();
		init_SPI2();
	}
	else {
		return;
	}
}

// transfer data out on output line and in on input line
void SPI::transfer(uint8_t b) {
	while ((_SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE); // wait for Transmit Buffer Empty flag to be set

	*(volatile uint8_t*)(&_SPIx->DR) = b; // write byte to data register

	while ((_SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY); // wait for busy to be unset
}

// read current data in spi register
void SPI::receive_byte(uint8_t* b) {
	while ((_SPIx->SR & SPI_SR_RXNE) != SPI_SR_RXNE); // wait for receive not empty to be set

	*b = *(volatile uint8_t*)(&_SPIx->DR); // read byte from data register
}

// configure PB3(SPI1_SCK), PB4(SPI1_MISO), PB5(SPI1_MOSI)
void SPI::init_SPI1_GPIO(void) {
	uint8_t af_num;

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //clk enabled

	// set to alternate function mode (10)
	// explanation: default is 11, so use mask &~(01) to set second bit to 0
	GPIOB->MODER &= ~(GPIO_MODER_MODER3_0 | 
					  GPIO_MODER_MODER4_0 | 
					  GPIO_MODER_MODER5_0);

	// reset AF for pin 3, 4, 5
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL3 | 
	                   GPIO_AFRL_AFSEL4 | 
	                   GPIO_AFRL_AFSEL5);

	// set pin 3, 4, 5 to AF5
	af_num = 5;
	GPIOB->AFR[0] |= (af_num << (3*4));
	GPIOB->AFR[0] |= (af_num << (4*4));
	GPIOB->AFR[0] |= (af_num << (5*4));

	// set to push-pull (0)
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT3 | 
	                   GPIO_OTYPER_OT4 | 
	                   GPIO_OTYPER_OT5);

	// set to very high (11)
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR3 | 
	                   GPIO_OSPEEDER_OSPEEDR4 | 
	                   GPIO_OSPEEDER_OSPEEDR5);
	
	// set to no pull-up, pull-down (00)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD3 | 
	                  GPIO_PUPDR_PUPD4 | 
	                  GPIO_PUPDR_PUPD5);
}

// initialize SPI1 peripheral as master
void SPI::init_SPI1(uint32_t freq){
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // enable SPI1 clock
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST; // set, then reset to clear SPI1
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST; 
	SPI1->CR1 &= ~SPI_CR1_SPE; // disable SPI1
	SPI1->CR1 &= ~SPI_CR1_RXONLY; // set to full-duplex (0)
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE; // set to 2-line unidirectional data mode (0)
	SPI1->CR1 &= ~SPI_CR1_BIDIOE; // disable output in bidirectional mode (0)
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST; // MSB first (0)
	SPI1->CR2 &= ~SPI_CR2_DS; // reset data size bits to 0000;
	SPI1->CR2 |= (SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0); // set data size to 8 (strangely, 8 corresponds to 0111 in the datasheet)
	SPI1->CR2 &= ~SPI_CR2_FRF; // use motorola SPI mode (0)
	SPI1->CR1 &= ~SPI_CR1_CPOL; // set clock to low polarity (0)
	SPI1->CR1 &= ~SPI_CR1_CPHA; // set clock to first clock transition (0)
	SPI1->CR1 &= ~SPI_CR1_BR; // reset baud rate control bits to 000

	// set spi clock prescalar to be the next highest requested freq
	uint8_t br;
	br = floor((log(80*1000000.0/freq)/log(2.0))-1);
	if (br < 0)
		br = 0;
	else if (br > 7)
		br = 7;
	SPI1->CR1 |= br << 3;

	SPI1->CR1 &= ~SPI_CR1_CRCEN; // disable hardware CRC calculation (0)
	SPI1->CR1 |= SPI_CR1_MSTR; // set to master (1)
	SPI1->CR1 |= SPI_CR1_SSM; // enable SSM (software slave management) (1)
	SPI1->CR2 |= SPI_CR2_NSSP; // enable NSS pulse generation (1)
	SPI1->CR1 |= SPI_CR1_SSI; // set internal slave select bit (1)
	SPI1->CR2 |= SPI_CR2_FRXTH; // set FIFO threshold to 1/4 (1)
	SPI1->CR1 |= SPI_CR1_SPE; // enable SPI1
}

// initialize SPI2 GPIO pins
void SPI::init_SPI2_GPIO(void) {
	uint32_t af_num;
	uint32_t pin_num;

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //clk enabled
	// configure PB13, PB14, PB15

	// set to alternate function mode (10)
	// explanation: default is 11, so use mask &~(01) to set second bit to 0
	GPIOB->MODER &= ~(GPIO_MODER_MODER13_0 | 
					  GPIO_MODER_MODER14_0 | 
					  GPIO_MODER_MODER15_0);

	// reset AF for pin 13, 14, 15
	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL13 | 
	                   GPIO_AFRH_AFSEL14 | 
	                   GPIO_AFRH_AFSEL15);

	// set pin 13, 14, 15 to AF5
	af_num = 5;
	pin_num = 13;
	GPIOB->AFR[1] |= (af_num << ((pin_num%8)*4));
	pin_num = 14;
	GPIOB->AFR[1] |= (af_num << ((pin_num%8)*4));
	pin_num = 15;
	GPIOB->AFR[1] |= (af_num << ((pin_num%8)*4));

	// set to push-pull (0)
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT13 | 
	                   GPIO_OTYPER_OT14 | 
	                   GPIO_OTYPER_OT15);

	// set to very high (11)
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR13 | 
	                   GPIO_OSPEEDER_OSPEEDR14 | 
	                   GPIO_OSPEEDER_OSPEEDR15);
	
	// set to no pull-up, pull-down (00)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD13 | 
	                  GPIO_PUPDR_PUPD14 | 
	                  GPIO_PUPDR_PUPD15);
}

// initialize SPI2 peripheral as slave
void SPI::init_SPI2(void){
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; // enable SPI2 clock
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST; // set, then reset to clear SPI2
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI2RST; 
	SPI2->CR1 &= ~SPI_CR1_SPE; // disable SPI2
	SPI2->CR1 &= ~SPI_CR1_RXONLY; // set to full-duplex (0)
	SPI2->CR1 &= ~SPI_CR1_BIDIMODE; // set to 2-line unidirectional data mode (0)
	SPI2->CR1 &= ~SPI_CR1_BIDIOE; // disable output in bidirectional mode (0)
	SPI2->CR1 &= ~SPI_CR1_LSBFIRST; // MSB first (0)
	SPI2->CR2 &= ~SPI_CR2_DS; // reset data size bits to 0000;
	SPI2->CR2 |= (SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0); // set data size to 8 (strangely, 8 corresponds to 0111 in the datasheet)
	SPI2->CR2 &= ~SPI_CR2_FRF; // use motorola SPI mode (0)
	SPI2->CR1 &= ~SPI_CR1_CPOL; // set clock to low polarity (0)
	SPI2->CR1 &= ~SPI_CR1_CPHA; // set clock to first clock transition (0)
	SPI2->CR1 &= ~SPI_CR1_BR; // reset baud rate control bits to 000
	SPI2->CR1 |= (SPI_CR1_BR_1 | SPI_CR1_BR_0); // set to 011 (prescalar value 16)
	SPI2->CR1 &= ~SPI_CR1_CRCEN; // disable hardware CRC calculation (0)
	SPI2->CR1 &= ~SPI_CR1_MSTR; // set to slave (0)
	SPI2->CR1 |= SPI_CR1_SSM; // enable SSM (software slave management) (1)
	SPI2->CR2 |= SPI_CR2_NSSP; // enable NSS pulse generation (1)
	SPI2->CR1 &= ~SPI_CR1_SSI; // set internal slave select bit (0)
	SPI2->CR2 |= SPI_CR2_FRXTH; // set FIFO threshold to 1/4 (1)
	SPI2->CR1 |= SPI_CR1_SPE; // enable SPI2
}
