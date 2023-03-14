#include "hardware_spi2.h"

HardwareSpi2::HardwareSpi2()
    : SPI(SPI2) {}

void HardwareSpi2::configureGpio() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIOB

	// set to alternate function mode (10)
	GPIOB->MODER &= ~(GPIO_MODER_MODER13 | // PB13
					  GPIO_MODER_MODER14 | // PB14
					  GPIO_MODER_MODER15); // PB15
	GPIOB->MODER |=  (GPIO_MODER_MODER13_1 | 
					  GPIO_MODER_MODER14_1 | 
					  GPIO_MODER_MODER15_1); // set to Alternate function mode
	GPIOB->MODER &=  ~GPIO_MODER_MODER12; 	// PB12
	GPIOB->MODER |=   GPIO_MODER_MODER12_0; // set to General purpose output mode

	// reset AF for PB13, PB14, PB15
	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL13 | // PB13
	                   GPIO_AFRH_AFSEL14 | // PB14
	                   GPIO_AFRH_AFSEL15); // PB15

	// set PB13, PB14, PB15 to AF15
	uint8_t af_num = 5;
	GPIOB->AFR[1] |= (af_num << ((13%8)*4)); // PB13
	GPIOB->AFR[1] |= (af_num << ((14%8)*4)); // PB14
	GPIOB->AFR[1] |= (af_num << ((15%8)*4)); // PB15

	// set to push-pull (0)
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT13 | 
	                   GPIO_OTYPER_OT14 | 
	                   GPIO_OTYPER_OT15);
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT12);

	// set to very high (11) //Original board uses 40MHz
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR13 | 
	                   GPIO_OSPEEDER_OSPEEDR14 | 
	                   GPIO_OSPEEDER_OSPEEDR15);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR12);
	
	// set to no pull-up, pull-down (00)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD13 | 
	                  GPIO_PUPDR_PUPD14 | 
	                  GPIO_PUPDR_PUPD15);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD12);
}

void HardwareSpi2::configureSpi() {
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; // enable SPI2 clock
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C2RST; // set, then reset to clear SPI2
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C2RST; 
	
	spix_->CR1 &= ~SPI_CR1_SPE; // disable SPI2	

	// ---------------CR1-----------------
	// set spi clock prescalar to be the next highest requested freq
	spix_->CR1 &= ~SPI_CR1_BR; // reset baud rate control bits to 000
	spix_->CR1 |= 0b110 << 3;

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

	// disable SSM (Hardware NSS management) (0)
	spix_->CR1 |= SPI_CR1_SSM;
	// set internal slave select to 1
	spix_->CR1 |= SPI_CR1_SSI;

	// set to master (1)
	spix_->CR1 |= SPI_CR1_MSTR;

	// ----------------CR2--------------
	// reset data size bits to 0000;
	spix_->CR2 &= ~SPI_CR2_DS;
	// set data size to 8 (8 corresponds to 0111 in the datasheet)
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

void HardwareSpi2::setCsLow() {
	GPIOB->ODR &= ~GPIO_ODR_OD12; // Set CS (PB12) to Low
}
void HardwareSpi2::setCsHigh() {
	GPIOB->ODR |= GPIO_ODR_OD12; // Set CS (PB12) to High
}
