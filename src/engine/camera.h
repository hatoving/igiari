#ifndef IGIARI_ENGINE_CAMERA_H
#define IGIARI_ENGINE_CAMERA_H

#include "../utils/glad.h"

typedef struct {
    float zoom;
    float x, y;
} igiari_engine_2dcam;
igiari_engine_2dcam* igiari_engine_2dcam_Create(float x, float y, float zoom);
float* igiari_engine_2dcam_GetMatrix(float left, float right, float top, float bottom);

#endif