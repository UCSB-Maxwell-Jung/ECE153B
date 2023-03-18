// Main program for ECE153B final project: digital camera
// click the button on the nucleo board to take a photo
// preview the photo on the LCD display
// save the photo to SD card simultaneously
 

#include "Adafruit_ILI9341.h"       // LCD display library
#include "SD.h"                     // SD card & FAT filesystem library
#include "Adafruit_ImageReader.h"   // Image-reading library

// declare hardware objects
SDClass SD;                         // SD card object
Adafruit_ImageReader reader(SD);    // Image-reader object, pass in SD filesys
Adafruit_ILI9341 tft;               // LCD object
Adafruit_Image       img;           // An image loaded into RAM
int32_t              width  = 0,    // BMP image dimensions
                     height = 0;

HardwareUsart1 camera;              // UART camera

void setup(void) {
    ImageReturnCode stat; // Status from image-reading functions

    Serial.begin(9600); // begin console output

    tft.begin(SPI_MAX_FREQ); // begin LCD
    Serial.println("Initializing filesystem...");
    if (!SD.begin()) { // begin SD card
        Serial.println("SD begin() failed");
        while (1); // Fatal error, do not continue
    }
    
    camera.begin(9600); // begin camera UART communication

    Serial.println("Camera initialized");

    // Fill screen blue. Not a required step, this just shows that we're
    // successfully communicating with the screen.
    tft.fillScreen(ILI9341_BLUE);

    camera.write(0x10); // command camera to capture image

    Serial.println("Done");
}

void loop(void) {
    uint8_t camera_byte = camera.read();
    Serial.println(camera_byte);
}