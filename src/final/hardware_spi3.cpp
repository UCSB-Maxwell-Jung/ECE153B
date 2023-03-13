#include "hardware_spi3.h"

HardwareSpi3::HardwareSpi3()
    : SPI(SPI3) {}

void HardwareSpi3::CsLow() {
	GPIOA->ODR &= ~GPIO_ODR_OD15; // Set CS (PA15) to Low
}
void HardwareSpi3::CsHigh() {
	GPIOA->ODR |= GPIO_ODR_OD15; // Set CS (PA15) to High
}

// Configure PC10(SPI3_SCK), PC11(SPI3_MISO), PC12(SPI3_MOSI), PA15(SPI3_NSS)
void HardwareSpi3::configureGpio() {
	uint8_t af_num;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // enable GPIOC
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable GPIOA

	// set to alternate function mode (10)
	GPIOC->MODER &= ~(GPIO_MODER_MODER10 | // PC10
					  GPIO_MODER_MODER11 | // PC11
					  GPIO_MODER_MODER12); // PC12
	GPIOC->MODER |=  (GPIO_MODER_MODER10_1 | // set to Alternate function mode
					  GPIO_MODER_MODER11_1 | 
					  GPIO_MODER_MODER12_1);
	GPIOA->MODER &=  ~GPIO_MODER_MODE15;   // PA15
	GPIOA->MODER |=   GPIO_MODER_MODE15_0; // set to General purpose output mode

	// reset AF for PA4, PC10, PC11, PC12
	GPIOC->AFR[1] &= ~(GPIO_AFRH_AFSEL10 | // PC10
	                   GPIO_AFRH_AFSEL11 | // PC11
	                   GPIO_AFRH_AFSEL12); // PC12

	// set PC10, PC11, PC12, PA4 to AF5
	af_num = 6;
	GPIOC->AFR[1] |= (af_num << ((10%8)*4)); // PC10
	GPIOC->AFR[1] |= (af_num << ((11%8)*4)); // PC11
	GPIOC->AFR[1] |= (af_num << ((12%8)*4)); // PC12

	// set to push-pull (0)
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT10 | 
	                   GPIO_OTYPER_OT11 | 
	                   GPIO_OTYPER_OT12);
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT15;

	// set to very high (11)
	GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10 | 
	                   GPIO_OSPEEDER_OSPEEDR11 | 
	                   GPIO_OSPEEDER_OSPEEDR12);
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;
	
	// set to no pull-up, pull-down (00)
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD10 | 
	                  GPIO_PUPDR_PUPD11 | 
	                  GPIO_PUPDR_PUPD12);
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD15;
}

// Initialize peripheral as master
void HardwareSpi3::configureSpi() {
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN; // enable SPI3 clock
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI3RST; // set, then reset to clear SPI3
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI3RST; 
	spix_->CR1 &= ~SPI_CR1_SPE; // disable SPI

	// ---------------CR1-----------------
	// set spi clock prescalar to be the next highest requested freq
	spix_->CR1 &= ~SPI_CR1_BR; // reset baud rate control bits to 000
	spix_->CR1 |= br_ << 3;

	// set clock polarity to low (0)
	spix_->CR1 &= ~SPI_CR1_CPOL;
	// set clock phase to first edge (0)
	spix_->CR1 &= ~SPI_CR1_CPHA;

	// set to full-duplex (0)
	spix_->CR1 &= ~SPI_CR1_RXONLY;
	// set to 2-line unidirectional data mode (0)
	spix_->CR1 &= ~SPI_CR1_BIDIMODE;
	// disable output in bidirectional mode (0)
	spix_->CR1 &= ~SPI_CR1_BIDIOE;

	// MSB first (0)
	spix_->CR1 &= ~SPI_CR1_LSBFIRST;
	
	// enable software slave management (1)
	spix_->CR1 |= SPI_CR1_SSM;
	// set internal slave select to 1
	spix_->CR1 |= SPI_CR1_SSI;

	// set to master (1)
	spix_->CR1 |= SPI_CR1_MSTR;

	// ----------------CR2--------------
	// reset data size bits to 0000;
	spix_->CR2 &= ~SPI_CR2_DS;
	// set data size to 8 (strangely, 8 corresponds to 0111 in the datasheet)
	spix_->CR2 |= (SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);

	// disable NSS output (0)
	spix_->CR2 &= ~SPI_CR2_SSOE;

	// use motorola SPI mode (0)
	spix_->CR2 &= ~SPI_CR2_FRF; 
	
	// set FIFO threshold to 1/4 (8-bit) (1)
	spix_->CR2 |= SPI_CR2_FRXTH;

	// disable hardware CRC calculation (0)
	spix_->CR1 &= ~SPI_CR1_CRCEN;

	// disable NSS pulse generation (0)
	spix_->CR2 &= ~SPI_CR2_NSSP;
}
