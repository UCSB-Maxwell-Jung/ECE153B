#include "intrpt_handler.h"
#include "SysTick.h"
#include "hardware_usart1.h"
#include "hardware_usart2.h"

#define MAX_IMAGE_SIZE 10000        // assume image is at most 10kB
#define CAMERA_BUFFER_CAPACITY MAX_IMAGE_SIZE
int8_t camera_serial_buffer[CAMERA_BUFFER_CAPACITY];
uint32_t buffer_position;

void capturePhoto(void) {
    console.println("Request to Capture Photo");
    camera_serial_interface.write(0x10); // send capture photo request to Arduino

    uint32_t image_size = camera_serial_interface.read(); // the first byte sent 
    for (int i = 0; i < 1000; i++) {
        console.println(camera_serial_interface.read());
    }
}

void saveCameraByte(void) {
    uint8_t c = camera_serial_interface.read();
    if (buffer_position < CAMERA_BUFFER_CAPACITY) {
        camera_serial_buffer[buffer_position++] = c;
    }
    else {
        console.println("camera buffer full!");
    }
}

