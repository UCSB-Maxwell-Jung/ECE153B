#include "SPI_Display.h"
#include "camera.h"

SPI_Display::SPI_Display()
    : SPI(ILI9341) {}

// Configure PB3(SPI1_SCK), PB4(SPI1_MISO), PB5(SPI1_MOSI), PA4(SPI1_NSS)
void SPI_Display::configure_GPIO() {
	uint8_t af_num;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable GPIOA

	// set to alternate function mode (10)
	// explanation: default is 11, so use mask &~(01) to set second bit to 0
	GPIOB->MODER &= ~(GPIO_MODER_MODER3_0 | // PB3
					  GPIO_MODER_MODER4_0 | // PB4
					  GPIO_MODER_MODER5_0); // PB5
	GPIOA->MODER &= ~(GPIO_MODER_MODER4_0); // PA4

	// reset AF for PA4, PB3, PB4, PB5
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL3 | // PB3
	                   GPIO_AFRL_AFSEL4 | // PB4
	                   GPIO_AFRL_AFSEL5); // PB5
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL4); // PA4


	// set PB3, PB4, PB5, PA4 to AF5
	af_num = 5;
	GPIOB->AFR[0] |= (af_num << (3*4)); // PB3
	GPIOB->AFR[0] |= (af_num << (4*4)); // PB4
	GPIOB->AFR[0] |= (af_num << (5*4)); // PB5
	GPIOA->AFR[0] |= (af_num << (4*4)); // PA4

	// set to push-pull (0)
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT3 | 
	                   GPIO_OTYPER_OT4 | 
	                   GPIO_OTYPER_OT5);
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT4);

	// set to very high (11)
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR3 | 
	                   GPIO_OSPEEDER_OSPEEDR4 | 
	                   GPIO_OSPEEDER_OSPEEDR5);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4);
	
	// set to no pull-up, pull-down (00)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD3 | 
	                  GPIO_PUPDR_PUPD4 | 
	                  GPIO_PUPDR_PUPD5);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD4);
}

// Initialize SPI1 peripheral as master
void SPI_Display::configure_SPI() {
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // enable SPI1 clock
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST; // set, then reset to clear SPI1
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST; 
	SPI1->CR1 &= ~SPI_CR1_SPE; // disable SPI1

	// ---------------CR1-----------------
	// set spi clock prescalar to be the next highest requested freq
	SPI1->CR1 &= ~SPI_CR1_BR; // reset baud rate control bits to 000
	SPI1->CR1 |= _br << 3;

	// set clock polarity to low (0)
	SPI1->CR1 &= ~SPI_CR1_CPOL;
	// set clock phase to first edge (0)
	SPI1->CR1 &= ~SPI_CR1_CPHA;

	// set to full-duplex (0)
	SPI1->CR1 &= ~SPI_CR1_RXONLY;
	// set to 2-line unidirectional data mode (0)
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
	// disable output in bidirectional mode (0)
	SPI1->CR1 &= ~SPI_CR1_BIDIOE;

	// MSB first (0)
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	
	// disable SSM (Hardware NSS management) (0)
	SPI1->CR1 &= ~SPI_CR1_SSM;

	// set to master (1)
	SPI1->CR1 |= SPI_CR1_MSTR;

	// ----------------CR2--------------
	// reset data size bits to 0000;
	SPI1->CR2 &= ~SPI_CR2_DS;
	// set data size to 8 (strangely, 8 corresponds to 0111 in the datasheet)
	SPI1->CR2 |= (SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);

	// set NSS output enable (1)
	SPI1->CR2 |= SPI_CR2_SSOE;

	// use motorola SPI mode (0)
	SPI1->CR2 &= ~SPI_CR2_FRF; 
	
	// set FIFO threshold to 1/4 (8-bit) (1)
	SPI1->CR2 |= SPI_CR2_FRXTH;

	// disable hardware CRC calculation (0)
	SPI1->CR1 &= ~SPI_CR1_CRCEN;

	// disable NSS pulse generation (0)
	SPI1->CR2 &= ~SPI_CR2_NSSP;
}
