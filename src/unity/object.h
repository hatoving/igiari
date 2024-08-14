#ifndef IGIARI_UNITY_OBJECT_H
#define IGIARI_UNITY_OBJECT_H

#include <stdint.h>
#include "serialized_file.h"

typedef struct {
    uint32_t path_id;
    int* version;
    int build_type;
    int platform;
    int type;
    int serialized_type;
    uint32_t byte_size;
} igiari_unity_object;

#endif