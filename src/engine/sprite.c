#include "camera.h"
#include "sprite.h"
#include "shader.h"
#include "core.h"

#include <math.h>
#include <stdlib.h>

igiari_engine_sprite* igiari_engine_sprite_Create(float x, float y, float scale_x, float scale_y, float rotation, igiari_engine_texture* tex) {
    igiari_engine_sprite* spr = (igiari_engine_sprite*)malloc(sizeof(igiari_engine_sprite));

    float vertices[] = {
        // positions (0-2) texture coords (3-4)
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // top left
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // top right
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f  // bottom left
    };
    unsigned int indices[] = {  
        0, 2, 1,
        0, 3, 2
    };

    glGenBuffers(1, &spr->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, spr->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &spr->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spr->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    spr->x = x;
    spr->y = y;
    spr->scale_x = scale_x;
    spr->scale_y = scale_y;
    spr->width = tex->width;
    spr->height = tex->height;
    
    spr->tex_x_offset = 0.0f;
    spr->tex_y_offset = 0.0f;
    spr->tex_w_offset = (float)spr->width;
    spr->tex_h_offset = (float)spr->height;
    
    spr->r = spr->g = spr->b = spr->a = 1.0f;

    spr->rotation = rotation;
    spr->texture = tex;

    return spr;
}

void igiari_engine_sprite_Draw(igiari_engine_sprite* spr, igiari_engine_shader* shader) {
    igiari_engine_shader_Use(shader);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spr->ebo);
    glBindTexture(GL_TEXTURE_2D, spr->texture->id);
    glUniform1i(1, 0);

    float* ortho = igiari_engine_2dcam_GetMatrix(
        IGIARI_ENGINE_CAMERA2D->x,
        ((float)IGIARI_ENGINE_TARGETSIZE_WIDTH) + IGIARI_ENGINE_CAMERA2D->x,
        IGIARI_ENGINE_CAMERA2D->y,
        ((float)IGIARI_ENGINE_TARGETSIZE_HEIGHT) + IGIARI_ENGINE_CAMERA2D->y
    );

    GLuint projectionMatrixLocation = glGetUniformLocation(shader->program_id, "uProjectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &ortho[0]);
    
    GLuint textureSizeLocation = glGetUniformLocation(shader->program_id, "uTextureSize");
    glUniform2f(textureSizeLocation, spr->texture->width, spr->texture->height);

    GLuint textureOffsetLocation = glGetUniformLocation(shader->program_id, "uTexCoordOffset");
    glUniform4f(textureOffsetLocation, spr->tex_x_offset, spr->tex_y_offset, spr->tex_w_offset, spr->tex_h_offset);
    
    float matrix[16] = {
        ((float)spr->width * spr->scale_x) * cos(spr->rotation), (-(float)spr->width * spr->scale_x) * sin(spr->rotation), 0.0f, 0.0f,
        (-(float)spr->height * spr->scale_y) * sin(spr->rotation), (-(float)spr->height * spr->scale_y) * cos(spr->rotation), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        spr->x, spr->y, 0.0f, 1.0f
    };
    unsigned int transform_loc = glGetUniformLocation(shader->program_id, "uModel");
    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, matrix);
    
    glUniform4f(glGetUniformLocation(shader->program_id, "uTintColor"), spr->r, spr->g, spr->b, spr->a);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}