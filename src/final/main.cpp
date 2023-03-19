// Main program for ECE153B final project: digital camera
// click the blue button on the nucleo board to take a photo
// preview the photo on the LCD display and save the photo to SD card

#include "SysClock.h"
#include "SysTick.h"
#include "LED.h"
#include "Button.h"
// #include "intrpt_handler.h"

#include "Adafruit_ILI9341.h"       // LCD display library
#include "SD.h"                     // SD card & FAT filesystem library
#include "Adafruit_ImageReader.h"   // Image-reading library
#include "hardware_usart1.h"
#include "camera_interface.h"

// declare hardware objects
SDClass SD;                         // SD card object
Adafruit_ImageReader reader(SD);    // Image-reader object, pass in SD filesys
Adafruit_ILI9341 tft;               // LCD object
Adafruit_Image       img;           // An image loaded into RAM
int32_t              width  = 0,    // BMP image dimensions
                     height = 0;

HardwareUsart2 console;             // UART console
HardwareUsart1 camera_serial_interface;              // UART camera

void setup(void);
void loop(void);
void saveImage(uint32_t i);

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
  while (1) {
    console.println("Initializing filesystem...");
    if (SD.begin()) { // begin SD card
      break;
    }
    console.println("SD begin() failed");
    delay(500);
  }
  
  
  // Fill screen black. Not a required step, this just shows that we're
  // successfully communicating with the screen.
  tft.fillScreen(ILI9341_BLACK);

  camera_serial_interface.begin(9600); // begin camera UART communication
  image_size = 0;
  pending_save = false;

  console.println("Camera initialized");
}

void loop(uint32_t i) {
  delay(500);
  toggleLed();
  if (pending_save) {
    saveImage(i);
  }
}

void saveImage(uint32_t i) {
  console.println("Saving captured image...");

  char filename[20];
  snprintf(filename, 20, "image_%d.jpg", i);

  File image_file = SD.open(filename, O_READ | O_WRITE | O_CREAT);

  // if the file opened okay, write to it:
  if (image_file) {
    console.print("Writing to ");
    console.print(filename);
    console.println("...");
    image_file.write(image_buffer, image_size);
    // close the file:
    image_file.close();
    console.print("Saved ");
    console.print(filename);
    console.println("!");
  } else {
    // if the file didn't open, print an error:
    console.print("error opening ");
    console.println(filename);
  }

  pending_save = false;
}

int main() {
  initHardware();
  setup();
  uint32_t i = 0;
  while(1) {
    loop(i);
    i++;
  }

  console.println("Critical Error!!!");
  return 1; // if program reaches this line, something went wrong
}