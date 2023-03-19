#include "Adafruit_ILI9341.h"       // LCD display library
#include "SD.h"                     // SD card & FAT filesystem library
#include "Adafruit_ImageReader.h"   // Image-reading library
#include "hardware_usart1.h"
#include "camera_interface.h"

#include "SysTick.h"
#include "LED.h"

#define console Serial
#define camera_serial_interface Serial1

// declare hardware objects
Adafruit_ImageReader reader(SD);    // Image-reader object, pass in SD filesys
Adafruit_ILI9341 tft;               // LCD object
Adafruit_Image       img;           // An image loaded into RAM
int32_t              width  = 0,    // BMP image dimensions
                     height = 0;

uint32_t loop_count = 0;

void drawImage(void);
void saveImage(void);

void setup(void) {
  ImageReturnCode stat; // Status from image-reading functions

  console.begin(9600); // begin console output

  tft.begin(SPI_MAX_FREQ); // begin LCD
  while (1) {
    Serial.println("Initializing filesystem...");
    if (SD.begin()) { // begin SD card
      break;
    }
    // retry after half a second if SD card fails
    Serial.println("SD begin() failed");
    delay(500);
  }

  tft.fillScreen(ILI9341_BLACK);

  // begin serial communication with Arduino+Camera subsystem
  camera_serial_interface.begin(9600);

  // reset global vars
  image_size = 0;
  new_image = false;

  // indicate Potato Cam is ready
  Serial.println("Potato Cam Ready!");
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
  Serial.println("Saving captured image...");

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
    Serial.print("Writing ");
    Serial.print(image_size);
    Serial.print(" bytes into ");
    Serial.print(filename);
    Serial.println("...");

    image_file.write(image_buffer, image_size);
    image_file.close();
    
    Serial.print("Saved ");
    Serial.print(filename);
    Serial.println("!");
  }
  else { // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(filename);
  }
}