#include "SPI.h"
#include <math.h>

SPI::SPI(SPI_TypeDef* SPIx)
	: spix_(SPIx) {}

void SPI::begin(uint32_t desired_freq) {
	desired_freq_ = desired_freq;
	// calculate prescalar exponent (prescalar = 2^(n+1))
	br_ = floor((log((float)CLK_FREQ/(float)desired_freq_)/log(2.0))-1);
	if (br_ < 0)
		br_ = 0;
	else if (br_ > 7)
		br_ = 7;
	actual_freq_ = CLK_FREQ/(1<<(br_+1)); // divide by 2^(n+1)

	configure_GPIO();
	configure_SPI();
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
	// wait for Transmit Buffer Empty flag to be set
	while ((spix_->SR & SPI_SR_TXE) != SPI_SR_TXE);
	// write byte to transfer
	*(volatile uint8_t*)(&spix_->DR) = b;

	// wait for data from slave
	while ((spix_->SR & SPI_SR_RXNE) != SPI_SR_RXNE);
	// read received byte
	return *(volatile uint8_t*)(&spix_->DR);
}
