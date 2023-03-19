#include "camera_interface.h"
#include "SysTick.h"
#include "hardware_usart1.h"
#include "hardware_usart2.h"

uint8_t image_buffer[IMAGE_BUFFER_CAPACITY];
volatile uint32_t image_size;
volatile bool new_image;

void capturePhoto(void) {
    image_size = 0;
    new_image = false;
    Serial.println("Capturing image...");
    Serial1.write(0x10); // send capture photo request to Arduino
    // assume image bytes start coming in immediately
    int camera_byte;
    while (image_size < IMAGE_BUFFER_CAPACITY) {
        camera_byte = Serial1.read(); // returns negative value on timeout
        if (camera_byte >= 0) {
            image_buffer[image_size++] = camera_byte;
            continue;
        }
        else { // if USART times out, assume end of image
            Serial.print("Captured image of size ");
            Serial.print(image_size);
            Serial.println(" bytes");
            new_image = true;
            return;
        }
    }
    
    Serial.print("Image too large (image size larger than ");
    Serial.print(IMAGE_BUFFER_CAPACITY);
    Serial.println(" bytes)");
}

// void saveCameraByte(void) {
//     uint8_t c = camera_serial_interface.read();
//     if (buffer_position < CAMERA_BUFFER_CAPACITY) {
//         camera_serial_buffer[buffer_position++] = c;
//     }
//     else {
//         Serial.println("camera buffer full!");
//     }
// }

