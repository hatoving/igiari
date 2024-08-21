#ifndef IGIARI_ENGINE_TEXTURE_H
#define IGIARI_ENGINE_TEXTURE_H

#include "../assets/unity/texture2d.h"
#include "../utils/glad.h"

typedef struct {
    int width;
    int height;

    GLuint id;
    char* data;
    int is_from_unity;
} igiari_engine_texture;
igiari_engine_texture* igiari_engine_texture_LoadFromPath(char* path);
igiari_engine_texture* igiari_engine_texture_LoadFromUnityAsset(igiari_unity_texture2d* tex);

#endif