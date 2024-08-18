#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

igiari_engine_shader* igiari_engine_shader_Create(const char* vertex_src, const char* frag_src) {
    igiari_engine_shader* shader = (igiari_engine_shader*)malloc(sizeof(igiari_engine_shader));

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_src, NULL);
    glCompileShader(vertex_shader);

    GLint success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("[igiari, engine] Error below: %s\n", info_log);
        free(shader);
        return NULL;
    }

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_src, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("[igiari, engine] Error below: %s\n", info_log);
        free(shader);
        return NULL;
    }

    shader->program_id = glCreateProgram();
    glAttachShader(shader->program_id, vertex_shader);
    glAttachShader(shader->program_id, frag_shader);
    glLinkProgram(shader->program_id);

    glGetProgramiv(shader->program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(shader->program_id, 512, NULL, info_log);
        printf("[igiari, engine] Failed to compile shader.\n\"%s\"", info_log);
        free(shader);
        return NULL;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    return shader;
}

void igiari_engine_shader_Use(igiari_engine_shader* shader) {
    glUseProgram(shader->program_id);
}