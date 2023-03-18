// Main program for ECE153B final project: digital camera
// click the blue button on the nucleo board to take a photo
// preview the photo on the LCD display and save the photo to SD card

#include "SysClock.h"
#include "SysTick.h"
#include "LED.h"
#include "Button.h"

#include "Adafruit_ILI9341.h"       // LCD display library
#include "SD.h"                     // SD card & FAT filesystem library
#include "Adafruit_ImageReader.h"   // Image-reading library
#include "hardware_usart1.h"

// declare hardware objects
SDClass SD;                         // SD card object
Adafruit_ImageReader reader(SD);    // Image-reader object, pass in SD filesys
Adafruit_ILI9341 tft;               // LCD object
Adafruit_Image       img;           // An image loaded into RAM
int32_t              width  = 0,    // BMP image dimensions
                     height = 0;

HardwareUsart2 console;             // UART console
HardwareUsart1 camera;              // UART camera

void setup(void);
void loop(void);

void initHardware() {
  initSystemClock();   // System Clock = 80 MHz
	initSysTick();
	initLed();
  initButton();
}

void setup(void) {
    ImageReturnCode stat; // Status from image-reading functions

    console.begin(9600); // begin console output

    tft.begin(SPI_MAX_FREQ); // begin LCD
    console.println("Initializing filesystem...");
    if (!SD.begin()) { // begin SD card
        console.println("SD begin() failed");
        while (1); // Fatal error, do not continue
    }
    
    camera.begin(9600); // begin camera UART communication

    console.println("Camera initialized");

    // Fill screen black. Not a required step, this just shows that we're
    // successfully communicating with the screen.
    tft.fillScreen(ILI9341_BLACK);

    console.println("Done");
}

void loop(void) {
    console.println(camera.read());
    ledToggle();
}

int main() {
  initHardware();
  setup();
  while(1)
    loop();

  return 1; // if program reaches this line, something went wrong
}