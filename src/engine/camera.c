#include "camera.h"
#include <stdlib.h>

float* igiari_engine_2dcam_GetMatrix(float left, float right, float top, float bottom) {
    float* result = (float*)malloc(16 * sizeof(float));

    float near = -1.0f; // Typically set to -1 for 2D
    float far = 1.0f;   // Typically set to 1 for 2D

    result[0] = 2.0f / (right - left);
    result[1] = 0.0f;
    result[2] = 0.0f;
    result[3] = 0.0f;

    result[4] = 0.0f;
    result[5] = 2.0f / (top - bottom);
    result[6] = 0.0f;
    result[7] = 0.0f;

    result[8] = 0.0f;
    result[9] = 0.0f;
    result[10] = -2.0f / (far - near);
    result[11] = 0.0f;

    result[12] = -(right + left) / (right - left);
    result[13] = -(top + bottom) / (top - bottom);
    result[14] = -(far + near) / (far - near);
    result[15] = 1.0f;

    return result;
}