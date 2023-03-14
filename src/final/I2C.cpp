#include "I2C.h"

I2C::I2C(I2C_TypeDef* I2Cx)
	: I2Cx_(I2Cx) {}

void I2C::begin(uint8_t address) {
	configureGpio();
	configureI2c();
}

uint8_t I2C::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
	// [TODO]

	return 0;
}

void I2C::beginTransmission(uint8_t address) {
	I2Cx_->CR2 &= ~I2C_CR2_SADD; // clear address
	I2Cx_->CR2 |= address << 1; // set address of targeted slave
	I2Cx_->CR2 &= ~I2C_CR2_RD_WRN; // request write transfer (0)
}

uint8_t I2C::endTransmission(uint8_t sendStop) {
	// [TODO]

	return 0;
}

int8_t I2C::write(uint8_t value) {
	write(&value, 1);
}

int8_t I2C::write(uint8_t data[], uint8_t quantity) {
	size_t i;
	I2Cx_->CR2 &= ~I2C_CR2_NBYTES;
	I2Cx_->CR2 |= quantity << 16; // set number of bytes to transfer

	I2Cx_->CR2 |= I2C_CR2_START; // start I2C

    for(i = 0; i < quantity; i++) {
		while((I2Cx_->ISR & I2C_ISR_TXIS) == 0) { // wait for previous transfer to complete
			if ((I2Cx_->ISR & I2C_ISR_NACKF) != 0)
				goto done; // exit if transfer is incomplete due to NACK
		} // wait for to finish transferring previous byte
		I2Cx_->TXDR = data[i];  // queue next byte to transfer
    }

done:
	return i;
}

uint8_t I2C::available() {
	// [TODO]
	
	return 0;
}

int8_t I2C::read() {
	// [TODO]

	return 0;
}
