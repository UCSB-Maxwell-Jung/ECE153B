#include "potato.h"

#include "Adafruit_ILI9341.h"       // LCD display library
#include "SD.h"                     // SD card & FAT filesystem library
#include "hardware_usart1.h"
#include "hardware_usart2.h"
#include "TJpg_Decoder.h"
#include "SysTick.h"
#include "LED.h"

#include "SysTick.h"
#include "hardware_usart1.h"
#include "hardware_usart2.h"

#define MAX_IMAGE_SIZE 16384 // assume max image size is 16kB (2^14)
#define IMAGE_BUFFER_CAPACITY MAX_IMAGE_SIZE
#define BUFFER_COUNT 2 // double buffer (one for storing last image, other for storing incoming image)

uint8_t image_buffer[BUFFER_COUNT][IMAGE_BUFFER_CAPACITY];
volatile uint32_t image_size[BUFFER_COUNT];
volatile uint8_t buffer_index;
volatile bool pending_save;

#define console Serial
#define camera Serial1

// declare hardware objects
Adafruit_ILI9341     tft;           // LCD object
int32_t              width  = 0,    // BMP image dimensions
                     height = 0;

uint32_t loop_count = 0;

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);

void setup(void) {
  console.begin(9600); // begin console output

  // begin serial communication with Arduino+Camera subsystem
  camera.begin(9000);
  // reset global vars/flags
  for (int i = 0; i < BUFFER_COUNT; i++) {
    image_size[i] = 0; // reset image size of all buffers
  }
  buffer_index = 0;
  pending_save = false;

  while (1) {
    if (SD.begin()) { // begin SD card
      break;
    }
    Serial.println("SD Card Error!");
    delay(500);
  }

  tft.begin(SPI_MAX_FREQ); // begin LCD

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
  bool new_image = captureImage();

  if (pending_save) { // flag set to true on button press
    saveImage();
  }

  if (new_image) {
    drawImage();
    buffer_index = (buffer_index + 1) % BUFFER_COUNT; // move to next buffer
  }

  resetCamera();
}

// command Arduino to capture image over USART1 TX
// read image sent by Arduino on USART1 RX
// return true if captured image is valid
// false if captured image is corrupted
bool captureImage(void) {
  ledOn();
  image_size[buffer_index] = 0;
  Serial.println("Capturing new image...");
  Serial1.write(0x10); // send capture photo request to Arduino

  // assume image bytes start coming in immediately
  int camera_byte;
  while (image_size[buffer_index] < IMAGE_BUFFER_CAPACITY) {
    camera_byte = Serial1.read();
    ledOff();
    if (camera_byte >= 0) { // valid bytes are non-negative
      image_buffer[buffer_index][image_size[buffer_index]++] = camera_byte;
      continue;
    }
    else { // exit loop on timeout (assume timeout = end of image)
      goto checkImage;
    }
  }

  // if loop was exited naturally, the image was too large
  Serial.print("Image larger than buffer size of ");
  Serial.print(IMAGE_BUFFER_CAPACITY);
  Serial.println(" bytes");

checkImage: 
  // Get the width and height in pixels of the jpeg to check for corruption
  uint16_t image_width = 0, image_height = 0;
  TJpgDec.getJpgSize(&image_width, &image_height, image_buffer[buffer_index], image_size[buffer_index]);
  bool image_corrupted = (image_width == 0) || (image_height == 0);
  if (image_corrupted)
    return false;

  Serial.print("Captured ");
  Serial.print(image_width); Serial.print(" x "); Serial.print(image_height);
  Serial.print(" image ");
  Serial.print("("); Serial.print(image_size[buffer_index]); Serial.println(" bytes)");
  return true;
}

// This function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // In ILI9341 library this function clips the image block at TFT boundaries
  tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}

void drawImage(void) {
  // Draw the image, top left at 0,0
  TJpgDec.drawJpg(0, 0, image_buffer[buffer_index], image_size[buffer_index]);
  Serial.println("Displayed captured image");
}

void saveImage(void) {
  Serial.println("Saving displayed image...");

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

  uint8_t previous_buffer_index = (buffer_index+BUFFER_COUNT-1)%BUFFER_COUNT; // index of previous buffer

  if (image_file) { // if the file opened okay, write to it:
    Serial.print("Writing ");
    Serial.print(image_size[previous_buffer_index]);
    Serial.print(" bytes into ");
    Serial.print(filename);
    Serial.println("...");

    image_file.write(image_buffer[previous_buffer_index], image_size[previous_buffer_index]);
    image_file.close();
    
    Serial.print("Saved ");
    Serial.print(filename);
    Serial.println("!");
    pending_save = false;
  }
  else { // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(filename);
  }
}

void resetCamera(void) {
  // send reset camera signal
  // delay(500);
}