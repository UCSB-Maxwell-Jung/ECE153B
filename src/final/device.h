/*
    This file contains mappings
    of various pheripherals to their 
    respective hardware interfaces.

    For example, DEV_PRINTER = TERMITE = USART2
*/

#ifndef __STM32L476R_NUCLEO_DEVICE_H
#define __STM32L476R_NUCLEO_DEVICE_H

#include "stm32l476xx.h"

#define BLUETOOTH USART1
#define TERMITE USART2

#define DEV_PRINTER TERMITE
#define DEV_DISPLAY SPI1

/*
    CS = chip select = PA3
    RESET = reset = PA2
    DC = Data / Command = PA10
    SDI(MOSI) = Serial Data In = SPI1 MOSI
    SCK = Serial Clock = SPI1 SCK

    SDO = Serial Data Out = NC (connect to SPI1 MISO for debugging)
*/

#endif