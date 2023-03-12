#include "nucleo.h"

UART_Wired Serial;

void init_hardware() {
  init_system_clock();   // System Clock = 80 MHz
	init_SysTick();
	init_LED();
}

int main() {
  init_hardware();
  setup();
  while(1)
    loop();

  return 1; // if program reaches this line, something went wrong
}