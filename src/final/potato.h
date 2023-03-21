#ifndef CAMERA_CONTROLS_H
#define CAMERA_CONTROLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

extern volatile bool pending_save;

bool captureImage(void);
void drawImage(void);
void saveImage(void);
void resetCamera();

#ifdef __cplusplus
}
#endif

#endif
