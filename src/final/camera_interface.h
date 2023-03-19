#ifndef CAMERA_CONTROLS_H
#define CAMERA_CONTROLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void capturePhoto(void);
// void saveCameraByte(void);

#define MAX_IMAGE_SIZE 16384 // assume max image size is 16kB (2^14)
#define IMAGE_BUFFER_CAPACITY MAX_IMAGE_SIZE
extern uint8_t image_buffer[IMAGE_BUFFER_CAPACITY];
extern volatile uint32_t image_size;
extern volatile bool new_image;

#ifdef __cplusplus
}
#endif

#endif
