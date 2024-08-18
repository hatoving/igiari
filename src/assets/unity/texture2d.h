#ifndef IGIARI_UNITY_TEXTURE2D_H
#define IGIARI_UNITY_TEXTURE2D_H

#include "bundle.h"
#include "object.h"

#include <raylib.h>
#include <stdint.h>

typedef struct {
    int filter_mode;
    int aniso;
    float mip_bias;
    int wrap_mode;
} igiari_unity_gltexsettings;

typedef struct {
    char* ptr;
    char* name;

    int forced_fallback_format;
    uint8_t downscale_fallback;

    int width, height;
    int texture_format;
    int size;

    uint8_t mip_map;
    int mip_count;

    uint8_t is_readable;
    int image_count, tex_dimension, lightmap_format, colorspace, data_size;

    igiari_unity_gltexsettings texture_settings;
    char* data;

    igiari_unity_object_streaminginfo info;
} igiari_unity_texture2d;
igiari_unity_texture2d* igiari_unity_texture2d_ReadFromPtr(const char* ptr);

igiari_unity_texture2d** igiari_unity_texture2d_GetAllTexFromNode(igiari_unity_bundle* bundle, char* path, int* tex_read);
igiari_unity_texture2d* igiari_unity_texture2d_GetTexByName(igiari_unity_texture2d** array, int size, char* name);

//Texture2D igiari_unity_texture2d_ConvertIntoRaylib(igiari_unity_texture2d* tex);

#endif