#include "texture.h"

#include <stdio.h>
#include <stdlib.h>

igiari_engine_texture* igiari_engine_texture_LoadFromUnityAsset(igiari_unity_texture2d* tex) {
    igiari_engine_texture* igiari_tex = (igiari_engine_texture*)malloc(sizeof(igiari_engine_texture));
    GLenum format;

    switch (tex->texture_format)
    {
        case 10: // DXT1
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
    
        default:
            format = GL_RGBA;
            break;
    }

    glGenTextures(1, &igiari_tex->id);
    glBindTexture(GL_TEXTURE_2D, igiari_tex->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //printf("making tesx (%i, %i)\n", gs1_logo_tex->width, gs1_logo_tex->height);
    if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) {
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, tex->width, tex->height, 0, tex->info.size, tex->data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, format, tex->width, tex->height, 0, format, GL_UNSIGNED_BYTE, tex->data);
    }
    //printf("done\n");
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //printf("%i, %i BITCH\n", tex->width, tex->height);
    igiari_tex->width = tex->width;
    igiari_tex->height = tex->height;

    igiari_tex->is_from_unity = 1;

    return igiari_tex;
}