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

void SPI::beginTransaction(void) {
	_SPIx->CR1 |= SPI_CR1_SPE; // enable spi
}

void SPI::endTransaction(void) {
	while ((_SPIx->SR & SPI_SR_FTLVL) != 0); // wait until no more data to transmit
	while ((_SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY); // wait until last data frame is processed
	_SPIx->CR1 &= ~SPI_CR1_SPE; // disable spi
	uint8_t discard;
	while ((_SPIx->SR & SPI_SR_FRLVL) != 0) // read all received data
		discard = *(volatile uint8_t*)(&_SPIx->DR); 
}

// transfer data out on output line and in on input line
uint8_t SPI::transfer(uint8_t b) {
	// wait for Transmit Buffer Empty flag to be set
	while ((_SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE);
	// write byte to transfer
	*(volatile uint8_t*)(&_SPIx->DR) = b;

	// wait for data from slave
	while ((_SPIx->SR & SPI_SR_RXNE) != SPI_SR_RXNE);
	// read received byte
	return *(volatile uint8_t*)(&_SPIx->DR);
}
