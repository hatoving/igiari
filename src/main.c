#include <stdio.h>
#include <string.h>

#include <malloc.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>

#include "assets/pwaat/mdt.h"
#include "assets/fmod/rebuild.h"

#include "assets/unity/serialized_file.h"
#include "assets/unity/bundle.h"
#include "assets/unity/audio_clip.h"
#include "assets/unity/texture2d.h"

#include "utils/glad.h"

#include "engine/camera.h"
#include "engine/texture.h"
#include "engine/sprite.h"
#include "engine/shader.h"
#include "engine/audio.h"
#include "engine/imgui.h"
#include "engine/core.h"

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
    igiari_engine_core_Initialize("igiari", 1280, 720, 1920, 1080, 60);
    igiari_imgui_CreateContext(IGIARI_ENGINE_SDLWINDOW, IGIARI_ENGINE_GLCONTEXT);
    
    ImFont* debug_font = igiari_imgui_AddFontFromFileTTF("ARIAL.TTF", 20.0f, NULL, NULL);
    ImFont* font = igiari_imgui_AddFontFromFileTTF("font.otf", 40.0f, NULL, NULL);
    
    igiari_unity_bundle* gs1_logo_bundle = igiari_unity_bundle_Read("titlegs1u.unity3d"); int game_bg_tex_count = 0;
    igiari_unity_texture2d** gs1_logo_texarray = igiari_unity_texture2d_GetAllTexFromNode(gs1_logo_bundle, "CAB-1f36da66d6416727fb8d0b18cb649fae", &game_bg_tex_count);
    igiari_unity_texture2d* gs1_logo_tex = igiari_unity_texture2d_GetTexByName(gs1_logo_texarray, game_bg_tex_count, "titleGS1u");

    igiari_unity_bundle* bg_bundle = igiari_unity_bundle_Read("title_back.unity3d"); int bg_tex_count = 0;
    igiari_unity_texture2d** bg_texarray = igiari_unity_texture2d_GetAllTexFromNode(bg_bundle, "CAB-ae9ba770903927e9f56b334635169106", &bg_tex_count);
    igiari_unity_texture2d* bg_tex = igiari_unity_texture2d_GetTexByName(bg_texarray, bg_tex_count, "title_back");

    free(gs1_logo_bundle);
    free(gs1_logo_texarray);

    igiari_engine_shader* shader = igiari_engine_shader_Create(VERTEX_SHADER_SRC, FRAG_SHADER_SRC);

    igiari_engine_sprite* gs1_logo = igiari_engine_sprite_Create(0.0f, -80.0f, 1.0f, 1.0f, 0.0f, igiari_engine_texture_LoadFromUnityAsset(gs1_logo_tex));
    igiari_engine_sprite* bg = igiari_engine_sprite_Create(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, igiari_engine_texture_LoadFromUnityAsset(bg_tex));

    free(bg_tex);
    free(bg_bundle);
    free(bg_texarray);
    free(gs1_logo_tex);

    igiari_unity_bundle* bgm_bundle = igiari_unity_bundle_Read("bgm200.unity3d"); int mus_count = 0;
    igiari_unity_audioclip** bgm_array = igiari_unity_audioclip_GetAllClipsFromNode(bgm_bundle, "CAB-b51dcfc932ed9c0b84264effd46acc63", &mus_count);
    igiari_audio_music* bgm_source = igiari_audio_CreateMusicFromAudioClip(igiari_unity_audioclip_GetClipByName(bgm_array, mus_count, "bgm200"));

    free(bgm_bundle);
    free(bgm_array);
    
    Mix_PlayMusicStream(bgm_source->src, -1);
    //Mix_SetMusicPositionStream(bgm_source->src, bgm_source->loop_end - (double)6.0f);
    
    SDL_Event e;
    while(IGIARI_ENGINE_RUNNING)
    {
        igiari_engine_core_StartUpdate(&e);

        igiari_engine_core_StartRender();
            igiari_engine_shader_Use(shader);
            igiari_engine_sprite_Draw(bg, shader);
            igiari_engine_sprite_Draw(gs1_logo, shader);
            
            igiari_imgui_NewFrame();
                igiari_imgui_SetNextWindowBgAlpha(0.0f);
                igiari_imgui_SetNextWindowPos(0.0f, 0.0f, 1 << 0, 0.0f, 0.0f);
                igiari_imgui_SetNextWindowSize(
                    (float)IGIARI_ENGINE_TARGETSIZE_WIDTH * ((float)IGIARI_ENGINE_WINDOW_WIDTH / (float)IGIARI_ENGINE_TARGETSIZE_WIDTH),
                    (float)IGIARI_ENGINE_TARGETSIZE_HEIGHT * ((float)IGIARI_ENGINE_WINDOW_HEIGHT / (float)IGIARI_ENGINE_TARGETSIZE_HEIGHT),
                    1 << 0
                );
                igiari_imgui_Begin("##text", NULL, igiari_imgui_WindowFlags_NoTitleBar | igiari_imgui_WindowFlags_NoResize | igiari_imgui_WindowFlags_NoScrollbar | igiari_imgui_WindowFlags_NoMove | igiari_imgui_WindowFlags_NoCollapse);
                //igiari_imgui_Begin("Phoenix", NULL, 0);
                    igiari_imgui_PushFont(font);
                    igiari_imgui_SetCursorPos(
                        ((20.0f) * ((float)IGIARI_ENGINE_VIEWPORT_WIDTH / (float)IGIARI_ENGINE_TARGETSIZE_WIDTH)) + IGIARI_ENGINE_VIEWPORT_OFFSET_X,
                        ((720.0f) * ((float)IGIARI_ENGINE_VIEWPORT_HEIGHT / (float)IGIARI_ENGINE_TARGETSIZE_HEIGHT)) + IGIARI_ENGINE_VIEWPORT_OFFSET_Y
                    );
                    igiari_imgui_PushStyleColor(0, 255, 255, 255, 255); igiari_imgui_Text("I saw my ");
                    igiari_imgui_SameLine(0.0f, 0.0f); igiari_imgui_PushStyleColor(0, 255, 0, 0, 255); igiari_imgui_Text("problems ");
                    igiari_imgui_SameLine(0.0f, 0.0f); igiari_imgui_PushStyleColor(0, 255, 255, 255, 255); igiari_imgui_Text("and I'll see the night. ");
                    igiari_imgui_SameLine(0.0f, 0.0f); igiari_imgui_PushStyleColor(0, 255, 255, 255, 255); igiari_imgui_PopStyleColor(); 
                    igiari_imgui_PopFont();
                    
                    igiari_imgui_PushFont(debug_font);
                    //igiari_imgui_Text("XOOOOOO");
                    //igiari_imgui_PushFont(font);
                    igiari_imgui_ShowDemoWindow(); 
                    igiari_imgui_PopFont();
                igiari_imgui_End();
            igiari_imgui_Render();
        igiari_engine_core_EndRender();

        igiari_engine_core_EndUpdate();
    
    }
    //Mix_FreeMusic(bgm_source->src);
    igiari_imgui_DestroyContext();
    igiari_engine_core_Quit();

    return 0;
}