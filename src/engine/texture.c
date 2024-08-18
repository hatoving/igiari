#include "texture.h"

#include <stdlib.h>

igiari_engine_texture* igiari_engine_texture_LoadFromUnityAsset(igiari_unity_texture2d* tex) {
    igiari_engine_texture* igiari_tex = (igiari_engine_texture*)malloc(sizeof(igiari_engine_texture));

    glGenTextures(1, &igiari_tex->id);
    glBindTexture(GL_TEXTURE_2D, igiari_tex->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //printf("making tesx (%i, %i)\n", gs1_logo_tex->width, gs1_logo_tex->height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->data);
    //printf("done\n");
    glGenerateMipmap(GL_TEXTURE_2D);

    igiari_tex->width = tex->width;
    igiari_tex->height = tex->height;

    igiari_tex->is_from_unity = 1;

    return igiari_tex;
}