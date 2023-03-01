/*  This file contains mappings
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

#endif