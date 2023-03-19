#include "Adafruit_ILI9341.h"       // LCD display library
#include "SD.h"                     // SD card & FAT filesystem library
#include "hardware_usart1.h"
#include "hardware_usart2.h"
#include "camera_interface.h"
#include "TJpg_Decoder.h"
#include "camera_interface.h"

#include "SysTick.h"
#include "LED.h"

#define console Serial
#define camera Serial1

// declare hardware objects
Adafruit_ILI9341     tft;           // LCD object
int32_t              width  = 0,    // BMP image dimensions
                     height = 0;

uint32_t loop_count = 0;

bool drawImage(void);
void saveImage(void);
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);

void setup(void) {
  console.begin(9600); // begin console output

  // begin serial communication with Arduino+Camera subsystem
  camera.begin(9600);
  // reset global vars
  image_size = 0;
  new_image = false;

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
  tft.setRotation(1);

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);
  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);

  // indicate Potato Cam is ready
  Serial.println("Potato Cam Ready!");
}

void loop(void) {
  if (((loop_count++) % 1000000) == 0) {
    toggleLed(); // blink LED every 100000 loop
  }
  capturePhoto();
  if (new_image) {
    if (drawImage()) { // attempt to draw new image
      Serial.println("Image displayed!");
      // saveImage(); // save if drawing successful
    }
    else { // 
      Serial.println("Image corrupted :(");
      Serial.println("Trying again after some delay");
    }
    new_image = false;
  }
}

// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // In ILI9341 library this function clips the image block at TFT boundaries
  tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}

bool drawImage(void) {
  // Get the width and height in pixels of the jpeg if you wish
  uint16_t w = 0, h = 0;
  TJpgDec.getJpgSize(&w, &h, image_buffer, image_size);
  Serial.print("width x height = "); Serial.print(w); Serial.print(" x "); Serial.println(h);

  // Draw the image, top left at 0,0
  TJpgDec.drawJpg(0, 0, image_buffer, image_size);

  // return false if image is 0 dimension (corrupted)
  return (w != 0) && (h != 0);
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