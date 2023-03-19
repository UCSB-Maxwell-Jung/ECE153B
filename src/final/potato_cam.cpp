#include "Adafruit_ILI9341.h"       // LCD display library
#include "SD.h"                     // SD card & FAT filesystem library
#include "Adafruit_ImageReader.h"   // Image-reading library
#include "hardware_usart1.h"
#include "camera_interface.h"

#include "SysTick.h"
#include "LED.h"

// declare hardware objects
Adafruit_ImageReader reader(SD);    // Image-reader object, pass in SD filesys
Adafruit_ILI9341 tft;               // LCD object
Adafruit_Image       img;           // An image loaded into RAM
int32_t              width  = 0,    // BMP image dimensions
                     height = 0;

HardwareUsart2 console;             // UART console
HardwareUsart1 camera_serial_interface;              // UART camera

void drawImage(void);
void saveImage(void);

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
  new_image = false;

  console.println("Potato Cam Ready!");
}

void loop(void) {
  delay(500);
  toggleLed();
  if (new_image) {
    drawImage();
    saveImage();
    new_image = false;
  }
}

void drawImage(void) {

}

void saveImage(void) {
  console.println("Saving captured image...");

  // Create a name for the new file in the format IMAGExy.JPG
  char filename[15];
  strcpy(filename, "IMAGE_00.JPG");
  for(int i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    if(!SD.exists(filename)) {
      break;
    }
  }

  File image_file = SD.open(filename, FILE_WRITE);

  if (image_file) { // if the file opened okay, write to it:
    console.print("Writing ");
    console.print(image_size);
    console.print(" bytes into ");
    console.print(filename);
    console.println("...");
    image_file.write(image_buffer, image_size);
    image_file.close();
    console.print("Saved ");
    console.print(filename);
    console.println("!");
  }
  else { // if the file didn't open, print an error:
    console.print("error opening ");
    console.println(filename);
  }
}