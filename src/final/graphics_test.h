#ifndef GRAPHICS_TEST_H
#define GRAPHICS_TEST_H

#include <stdint.h>

// Maximum SPI frequency the display can run at (determined from trial and error)
#define ILI9341_MAX_FREQ 40000000 // 40Mhz

void setup(void);
void loop(void);

#endif