#include <stdio.h>
#include <string.h>

#include "mdt.h"

#include "assets/unity/serialized_file.h"
#include "assets/unity/bundle.h"
#include "assets/unity/audio_clip.h"
#include "assets/unity/texture2d.h"

#include "assets/fmod/rebuild.h"

#include <malloc.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include "utils/glad.h"

#include "engine/camera.h"
#include "engine/texture.h"
#include "engine/sprite.h"
#include "engine/shader.h"
#include "engine/core.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const char* VERTEX_SHADER_SRC = R"(
    #version 330 core
    layout (location = 0) in vec4 aPosition;
    layout (location = 1) in vec2 aTexCoord;

    uniform mat4 uProjectionMatrix;
    uniform mat4 uModel;

    varying vec2 v_texCoord;

    void main()
    {
        gl_Position = uProjectionMatrix * uModel * aPosition;
        v_texCoord = aTexCoord;
    }
)";

const char* FRAG_SHADER_SRC = R"(
    #version 330 core
    out vec4 FragColor;

    varying vec2 v_texCoord;
    uniform sampler2D u_texture;

    void main()
    {
        gl_FragColor = texture(u_texture, v_texCoord);
    }
)";

int main(int argc,char **argv) {
    igiari_engine_core_Initialize("igiari", 1280, 720, 60);

    igiari_unity_bundle* gs1_logo_bundle = igiari_unity_bundle_Read("titlegs1u.unity3d");
    int game_bg_tex_count = 0;
    igiari_unity_texture2d** gs1_logo_texarray = igiari_unity_texture2d_GetAllTexFromNode(gs1_logo_bundle, "CAB-1f36da66d6416727fb8d0b18cb649fae", &game_bg_tex_count);
    igiari_unity_texture2d* gs1_logo_tex = igiari_unity_texture2d_GetTexByName(gs1_logo_texarray, game_bg_tex_count, "titleGS1u");

    free(gs1_logo_bundle);
    free(gs1_logo_texarray);

    igiari_engine_shader* shader = igiari_engine_shader_Create(VERTEX_SHADER_SRC, FRAG_SHADER_SRC);
    igiari_engine_sprite* gs1_logo = igiari_engine_sprite_Create(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, igiari_engine_texture_LoadFromUnityAsset(gs1_logo_tex));

    SDL_Event e;
    while(IGIARI_ENGINE_RUNNING)
    {
        igiari_engine_core_StartUpdate(&e);

        igiari_engine_core_StartRender();
            igiari_engine_shader_Use(shader);
            igiari_engine_sprite_Draw(gs1_logo, shader);
        igiari_engine_core_EndRender();

        igiari_engine_core_EndUpdate();
    }

    igiari_engine_core_Quit();

    return 0;
}