#include "camera_controls.h"
#include "hardware_usart1.h"
#include "hardware_usart2.h"

void capture_photo(void) {
    console.println("Capture Photo");
    camera.write(0x10);
}