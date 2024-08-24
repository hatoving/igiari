#include "sprite.h"

const char* IGIARI_SHADERS_SPRITE_VERT = R"(
    #version 330 core
    layout (location = 0) in vec4 aPosition;
    layout (location = 1) in vec2 aTexCoord;

    uniform mat4 uProjectionMatrix;
    uniform vec2 uTextureSize;
    uniform vec4 uTexCoordOffset;
    uniform vec4 uTintColor;
    uniform mat4 uModel;
    
    out vec4 v_tintColor;
    out vec2 v_texCoord;

    void main()
    {
        gl_Position = uProjectionMatrix * uModel * aPosition;

        vec2 offset = uTexCoordOffset.xy / uTextureSize;
        vec2 scale = uTexCoordOffset.zw / uTextureSize;
        
        v_tintColor = uTintColor;
        v_texCoord = aTexCoord * scale + offset;
    }
)";

const char* IGIARI_SHADERS_SPRITE_FRAG = R"(
    #version 330 core
    out vec4 FragColor;

    varying vec2 v_texCoord;
    uniform sampler2D u_texture;
    in vec4 v_tintColor;

    void main()
    {
        FragColor = texture(u_texture, v_texCoord) * v_tintColor;  
    }
)";