#include "nucleo.h"

HardwareUsart2 Serial;

void initHardware() {
  initSystemClock();   // System Clock = 80 MHz
	initSysTick();
	initLed();
}

int main() {
  initHardware();
  setup();
  while(1)
    loop();

  return 1; // if program reaches this line, something went wrong
}