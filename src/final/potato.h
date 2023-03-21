#ifndef CAMERA_CONTROLS_H
#define CAMERA_CONTROLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool captureImage(void);
void drawImage(void);
void saveImage(void);

#ifdef __cplusplus
}
#endif

#endif
