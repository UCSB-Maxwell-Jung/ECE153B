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

HardwareUsart1 camera_serial_interface;              // UART camera

uint32_t loop_count = 0;

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
    // retry after half a second if SD card fails
    console.println("SD begin() failed");
    delay(500);
  }

  tft.fillScreen(ILI9341_BLACK);

  // begin serial communication with Arduino+Camera subsystem
  camera_serial_interface.begin(9600);

  // reset global vars
  image_size = 0;
  new_image = false;

  // indicate Potato Cam is ready
  console.println("Potato Cam Ready!");
}

void loop(void) {
  if (((loop_count++) % 1000000) == 0) {
    toggleLed(); // blink LED every 100000 loop
  }
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

  // Create a name for the new file in the format IMAGE_##.JPG
  char filename[15];
  strcpy(filename, "IMAGE_00.JPG");
  for(int i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    if(!SD.exists(filename)) {
      break;
    }
  }

  // create new image file in SD card
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