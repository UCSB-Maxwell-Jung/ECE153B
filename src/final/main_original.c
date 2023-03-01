/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 2A
 */

/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "UART.h"
#include "LED.h"
#include "SysTimer.h"
#include "SysClock.h"
// #include "Adafruit_GFX.h"
// #include "Adafruit_ILI9341.h"
#include <string.h>
#include <stdio.h>


#define BAUD_RATE 9600

void setup(void) {
  init_system_clock(); // Switch System Clock = 80 MHz
	begin_serial(TERMITE, 9600);
	init_LED();
}

void loop(void) {
  char command[64]; // assume command typed by the user is at most 63 characters (63 + 1 null character)
  printf("Please enter a command:");
  scanf("%s", command);
  
  if((strcmp(command, "Y") == 0) || (strcmp(command, "y") == 0)){
    printf("LED ON \n");
    Green_LED_On();
  }
  else if((strcmp(command, "N") == 0) || (strcmp(command, "n") == 0)){
    printf("LED OFF \n");
    Green_LED_Off();
  }
  else {
    printf("Please input valid character: 'y'/'Y' or 'n'/'N' \n");
  }
}

int main() {
  setup();
  while(1)
    loop();
}