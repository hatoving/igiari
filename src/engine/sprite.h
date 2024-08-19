#ifndef IGIARI_ENGINE_SPRITE_H
#define IGIARI_ENGINE_SPRITE_H

#include "texture.h"
#include "shader.h"
#include "../utils/glad.h"

typedef struct {
    GLuint vbo;
    GLuint ebo;

    float x, y;
    float scale_x, scale_y;
    float width, height;
    float rotation;

    igiari_engine_texture* texture;
} igiari_engine_sprite;
igiari_engine_sprite* igiari_engine_sprite_Create(float x, float y, float scale_x, float scale_y, float rotation, igiari_engine_texture* tex);
void igiari_engine_sprite_Draw(igiari_engine_sprite* spr, igiari_engine_shader* shader);

#endif