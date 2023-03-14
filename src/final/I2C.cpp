#include "I2C.h"

I2C::I2C(I2C_TypeDef* I2Cx)
	: I2Cx_(I2Cx) {}

void I2C::begin(uint8_t address) {
	configureGpio();
	configureI2c();
	I2Cx_->CR2 &= ~I2C_CR2_AUTOEND; // force manual STOP control
}

// rd_wrn = 0 = write request
// rd_wrn = 1 = read request
void I2C::beginTransaction(uint8_t address, uint8_t quantity, bool rd_wrn) {
	I2Cx_->CR1 |= I2C_CR1_PE; // enable I2C
	I2Cx_->CR2 &= ~I2C_CR2_SADD; // clear address
	I2Cx_->CR2 |= address << 1; // set address of targeted slave
	if (rd_wrn)
		I2Cx_->CR2 |=  I2C_CR2_RD_WRN; // request read transfer (1)
	else
		I2Cx_->CR2 &= ~I2C_CR2_RD_WRN; // request write transfer (0)

	I2Cx_->CR2 &= ~I2C_CR2_NBYTES;
	I2Cx_->CR2 |= quantity << 16; // set number of bytes to transfer

	I2Cx_->CR2 |= I2C_CR2_START; // start I2C
}


uint8_t I2C::endTransaction(uint8_t sendStop) {
	if (sendStop == 0) { // RESTART
		I2Cx_->CR2 |= I2C_CR2_RELOAD; // reload number of bytes to transfer
		I2Cx_->CR2 |= I2C_CR2_START; // generate (RE)START
		return 0;
	}

	I2Cx_->CR2 |= I2C_CR2_STOP;  // generate STOP after current byte transfer
	while((I2Cx_->ISR & I2C_ISR_STOPF) == 0); // Wait until STOP flag is set
	I2Cx_->CR1 &= ~I2C_CR1_PE; // disable I2C

	return 0;
}

size_t I2C::read(uint8_t *data, size_t quantity) {
	size_t received;
    for (received = 0; received < quantity; received++) {
		while ((I2Cx_->ISR & I2C_ISR_RXNE) == 0) { // wait for receive to complete
			if ((I2Cx_->ISR & I2C_ISR_NACKF) != 0)
				return received; // exit if receive is incomplete due to NACK
		}
		data[received] = I2Cx_->RXDR;  // save received data
	}

	return received; // this value should match quantity if everything worked
}

size_t I2C::write(const uint8_t *data, size_t quantity) {
	size_t transmitted;
	// wait for any previous data to finish transferring
	while ((I2Cx_->ISR & I2C_ISR_TXIS) == 0) {
		if ((I2Cx_->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF)
			return -1; // address not transferred properly
	}
    for (transmitted = 0; transmitted < quantity; transmitted++) {
		I2Cx_->TXDR = data[transmitted];  // queue next byte to transmit
		while ((I2Cx_->ISR & I2C_ISR_TXIS) == 0) { // wait for transmit to complete
			if ((I2Cx_->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF)
				return transmitted; // exit if transmit is incomplete due to NACK
		}
	}

	return transmitted; // this value should match quantity if everything worked
}

// implement flowchart in figure 365 on pg 1124 of reference manual
uint8_t I2C::transmit(uint8_t address, uint8_t data[], uint8_t quantity, uint8_t sendStop) {
	uint8_t transmitted;
	beginTransaction(address, quantity, 0); // 0 for write request
    transmitted = write(data, quantity);
	endTransaction(sendStop);

	return transmitted;
}

// implement flowchart in figure 368 on pg 1128 of reference manual
uint8_t I2C::receive(uint8_t address, uint8_t data[], uint8_t quantity, uint8_t sendStop) {
	size_t received;
	beginTransaction(address, quantity, 1); // 1 for read request
    received = read(data, quantity);
	endTransaction(sendStop);

	return received;
}