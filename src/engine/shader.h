#ifndef IGIARI_ENGINE_SHADER_H
#define IGIARI_ENGINE_SHADER_H

#include "../utils/glad.h"

typedef struct {
    GLuint program_id;
} igiari_engine_shader;

igiari_engine_shader* igiari_engine_shader_Create(const char* vertex_src, const char* frag_src);
void igiari_engine_shader_Use(igiari_engine_shader* shader);

#endif