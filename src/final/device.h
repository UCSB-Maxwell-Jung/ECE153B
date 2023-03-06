/*
    This file contains mappings
    of various pheripherals to their 
    respective hardware interfaces.

    For example, DEV_TERMINAL = TERMITE = USART2
*/

#ifndef __STM32L476R_NUCLEO_DEVICE_H
#define __STM32L476R_NUCLEO_DEVICE_H

#include "stm32l476xx.h"

#define CLK_FREQ  80000000 // 80Mhz

#define BLUETOOTH USART1
#define TERMITE USART2

#define DEV_TERMINAL TERMITE
#define DISPLAY SPI1

/*
    termite
    UART2_TX = PA2
    UART2_RX = PA3
*/

/*
    ili9341 pins
    VDD - 5V
    GND - GND
    CS = chip select = SPI1 NSS = PA4
    RST = reset = PA8
    DC = Data / Command = PA10
    SDI(MOSI) = Serial Data In = SPI1 MOSI = PB5
    SCK = Serial Clock = SPI1 SCK = PB3
    LED = 5V
    SDO(MISO) = Serial Data Out = SPI1 MISO = PB4
*/

#endif