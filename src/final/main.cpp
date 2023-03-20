// Main program for ECE153B final project: digital camera
// click the blue button on the nucleo board to shoot a photo
// save the photo to SD card
// preview the most recent photo on the LCD display

#include "SysClock.h"
#include "SysTick.h"
#include "LED.h"
#include "Button.h"

#include "potato.h"

void initHardware() {
  initSystemClock();   // System Clock = 80 MHz
	initSysTick();
	initLed();
  initButton();
}

int main() {
  initHardware();
  setup();
  while(1) {
    loop();
  }

  return 1; // if program reaches this line, something went wrong
}