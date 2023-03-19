#include "camera_interface.h"
#include "SysTick.h"
#include "hardware_usart1.h"
#include "hardware_usart2.h"

uint8_t image_buffer[IMAGE_BUFFER_CAPACITY];
volatile uint32_t image_size;

void capturePhoto(void) {
    console.println("Capture image");
    camera_serial_interface.write(0x10); // send capture photo request to Arduino
    // assume image bytes start coming in immediately
    image_size = 0;
    while (image_size < IMAGE_BUFFER_CAPACITY) {
        image_buffer[image_size++] = camera_serial_interface.read();
        if (usart_timeout) { // if USART times out, assume end of image
            console.print("Captured ");
            console.print(image_size);
            console.println(" bytes of image data");
            return;
        }
    }

    console.print("Image too large (image size larger than ");
    console.print(IMAGE_BUFFER_CAPACITY);
    console.println(" bytes)");
}

// void saveCameraByte(void) {
//     uint8_t c = camera_serial_interface.read();
//     if (buffer_position < CAMERA_BUFFER_CAPACITY) {
//         camera_serial_buffer[buffer_position++] = c;
//     }
//     else {
//         console.println("camera buffer full!");
//     }
// }

