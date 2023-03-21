#include "potato.h"

// modded Arduino Libraries
#include "Adafruit_ILI9341.h"       // Adafruit LCD display library
#include "SD.h"                     // Arduino SD card & FAT filesystem library
#include "TJpg_Decoder.h"           // JPEG to Bitmap library

// custom libraries
#include "SysTick.h"
#include "button.h"
#include "LED.h"
#include "hardware_usart1.h"        // for Serial1
#include "hardware_usart2.h"        // for Serial

#define console Serial // remap extern variable Serial (declared in hardware_usart2.cpp)
#define camera Serial1 // remap extern variable Serial1 (declared in hardware_usart1.cpp)

#define MAX_IMAGE_SIZE 16384 // assume max image size is 16kB (2^14)
#define BUFFER_COUNT 2 // number of image buffers (in this case, double buffering)

uint8_t image_buffer[BUFFER_COUNT][MAX_IMAGE_SIZE]; // declare buffers in static memory
volatile uint32_t image_size[BUFFER_COUNT]; // variable for tracking size of each image in buffer
volatile uint8_t buffer_index; // index for selecting currently active buffer

#define pending_save button_pressed // remap extern variable button_pressed (declared in button.c)

// declare hardware objects as global variables
Adafruit_ILI9341     tft; // TFT LCD object
// SD card object is extern variable declared in SD.cpp

// rendering function for JPEG decoder libray
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);
void drawCaptureMessage(void);
void drawSdWarningMessage(void);
void drawHelpMessage(void);

void setup(void) {
  console.begin(9600); // begin console output

  camera.begin(9000); // begin serial communication with Arduino+Camera subsystem

  // reset image size of all buffers
  for (int i = 0; i < BUFFER_COUNT; i++) { 
    image_size[i] = 0;
  }
  // reset global variables/flags
  buffer_index = 0;
  pending_save = false;

  // loop until SD card initializes
  while (1) {
    if (SD.begin()) {
      break;
    }
    Serial.println("SD Card Error!");
    delay(500);
  }

  tft.begin(SPI_MAX_FREQ); // begin LCD

  tft.fillScreen(ILI9341_BLACK); // paint screen black
  tft.setRotation(1); // rotate display 90 degrees to landscape mode

  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(tft_output); // The decoder must be given the exact name of the rendering function

  // indicate Potato Cam is ready
  Serial.println("Potato Cam Ready!");
}

// capture new image, save old image currently on display
// before drawing the new image
void loop(void) {
  drawCaptureMessage();
  bool new_image = captureImage(); // returns false when image is corrupted
                                   // This function is the greatest bottleneck
                                   // returning after ~8 seconds

  if (pending_save) { // pending save is set to true on button press
    saveImage();
  }

  if (new_image) {
    drawImage();
    drawHelpMessage();
    buffer_index = (buffer_index + 1) % BUFFER_COUNT; // use next buffer
  }
}

// send 0x10 over USART1 to Arduino to initiate image capture 
// listen on USART1 for image data sent by Arduino and save image data into buffer.
// Assume end of image data when USART read times out
bool captureImage(void) {
  ledOn();
  image_size[buffer_index] = 0;
  Serial.println("Capturing new image...");
  Serial1.write(0x10); // send capture photo request to Arduino

  // immediately start listening for image data from Arduino
  int camera_byte;
  while (image_size[buffer_index] < MAX_IMAGE_SIZE) {
    camera_byte = Serial1.read(); // negative value on timeout
    ledOff();
    if (camera_byte >= 0) { // save byte to buffer sequentially
      image_buffer[buffer_index][image_size[buffer_index]++] = camera_byte;
      continue;
    }
    else { // exit loop on timeout (assume timeout = end of image)
      goto checkImage;
    }
  }

  // if loop exited naturally, the image was too large
  Serial.print("Image larger than buffer size of ");
  Serial.print(MAX_IMAGE_SIZE);
  Serial.println(" bytes");

checkImage: 
  // Get the width and height in pixels of the jpeg to check for corruption
  uint16_t image_width = 0, image_height = 0;
  TJpgDec.getJpgSize(&image_width, &image_height, image_buffer[buffer_index], image_size[buffer_index]);
  bool image_corrupted = (image_width == 0) || (image_height == 0);
  if (image_corrupted) {
    Serial.println("Image corrupted");
    return false;
  }

  Serial.print("Captured ");
  Serial.print(image_width); Serial.print(" x "); Serial.print(image_height);
  Serial.print(" image ");
  Serial.print("("); Serial.print(image_size[buffer_index]); Serial.println(" bytes)");
  return true;
}

void drawImage(void) {
  // Draw the image, top left at 0,0
  TJpgDec.drawJpg(0, 0, image_buffer[buffer_index], image_size[buffer_index]);
  Serial.println("Displayed captured image");
}

void saveImage(void) {
  Serial.println("Saving displayed image...");

  // Create a name for the new file in the format IMAGE_##.JPG
  char filename[20];
  for(int i = 0; true; i++) {
    sprintf(filename, "IMAGE_%d.JPG", i);
    if(!SD.exists(filename)) {
      break;
    }
  }

  Serial.print("Opening "); Serial.print(filename); Serial.println("...");
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
    
    Serial.print("Saved "); Serial.print(filename); Serial.print(" (");
    Serial.print(image_size[previous_buffer_index]); Serial.println(" bytes)");
    pending_save = false;
  }
  else { // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(filename);
    drawSdWarningMessage();
  }
}

// This function will be called during decoding of the jpeg file to
// render each block to the TFT LCD display.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // In ILI9341 library this function clips the image block at TFT boundaries
  tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}

void drawCaptureMessage(void) {
  tft.setTextSize(2);
  tft.setCursor(0, ILI9341_TFTWIDTH-16);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Capturing new image...");
}

void drawSdWarningMessage(void) {
  // show warning text on display
  tft.setTextSize(3);
  tft.setCursor(0, ILI9341_TFTWIDTH/2);
  tft.setTextColor(ILI9341_RED);
  tft.println("SD card Error!!!");
  delay(1000);
}

void drawHelpMessage(void) {
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_ORANGE);
  tft.print("Press ");
  tft.setTextColor(ILI9341_BLUE);
  tft.print("Blue Button ");
  tft.setTextColor(ILI9341_ORANGE);
  tft.println("to save this image");
}