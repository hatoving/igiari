#ifndef IGIARI_ENGINE_CAMERA_H
#define IGIARI_ENGINE_CAMERA_H

#include "../utils/glad.h"

typedef struct {
    float zoom;
    int x, y;
} igiari_engine_2dcam;
float* igiari_engine_2dcam_GetMatrix(float left, float right, float top, float bottom);

#endif