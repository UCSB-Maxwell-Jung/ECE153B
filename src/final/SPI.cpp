#include "SPI.h"
#include <math.h>

SPI::SPI(SPI_TypeDef* SPIx)
	: spix_(SPIx) {}

void SPI::begin(uint32_t desired_freq) {
	int8_t br;
	desired_freq_ = desired_freq;
	// calculate prescalar that results in the next fastest SPI frequency
	br = ceil((log((double)CLK_FREQ/(double)desired_freq)/log(2.0))-1);
	if (br < 0)
		br = 0;
	else if (br > 7)
		br = 7;
	br_ = br;
	actual_freq_ = CLK_FREQ/(1<<(br_+1)); // divide by 2^(n+1)

	configureGpio();
	configureSpi();
	setCsHigh();
}

void SPI::beginTransaction(void) {
	spix_->CR1 |= SPI_CR1_SPE; // enable spi
}

void SPI::endTransaction(void) {
	while ((spix_->SR & SPI_SR_FTLVL) != 0); // wait until no more data to transmit
	while ((spix_->SR & SPI_SR_BSY) == SPI_SR_BSY); // wait until last data frame is processed
	spix_->CR1 &= ~SPI_CR1_SPE; // disable spi
	uint8_t discard;
	while ((spix_->SR & SPI_SR_FRLVL) != 0) // read all received data
		discard = *(volatile uint8_t*)(&spix_->DR); 
}

// transfer data out on output line and in on input line
uint8_t SPI::transfer(uint8_t b) {
	uint8_t timeout;
	timeout = 0;
	// wait for Transmit Buffer Empty flag to be set
	while ((spix_->SR & SPI_SR_TXE) != SPI_SR_TXE) {
		timeout++;
		if (timeout>200)
			return 0;
	}
	// write byte to transfer
	*(volatile uint8_t*)(&spix_->DR) = b;

	timeout = 0;
	// wait for data from slave
	while ((spix_->SR & SPI_SR_RXNE) != SPI_SR_RXNE) {
		timeout++;
		if (timeout>200)
			return 0;
	}
	// read received byte
	return *(volatile uint8_t*)(&spix_->DR);
}
