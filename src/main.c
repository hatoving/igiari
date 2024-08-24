#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>

#include "assets/pwaat/mdt.h"

#include "assets/unity/bundle.h"
#include "assets/unity/audio_clip.h"
#include "assets/unity/texture2d.h"

#include "games/effects.h"
#include "games/trilogy/textbox.h"
#include "games/trilogy/sound_data.h"

#include "games/shaders/sprite.h"

#include "utils/glad.h"

#include "engine/camera.h"
#include "engine/texture.h"
#include "engine/sprite.h"
#include "engine/shader.h"
#include "engine/audio.h"
#include "engine/imgui.h"
#include "engine/core.h"

igiari_engine_sprite* LoadSpriteFromUnityBundle(char* bundle_name, char* node_name, char* tex_name) {
    igiari_engine_sprite* spr = NULL;
    
    igiari_unity_bundle* bundle = igiari_unity_bundle_Read(bundle_name);
    if (!bundle) return NULL;
    
    int count = 0;
    igiari_unity_texture2d** array = igiari_unity_texture2d_GetAllTexFromNode(bundle, node_name, &count);
    if (!array || count == 0) {
        igiari_unity_bundle_Free(bundle);
        return NULL;
    }
    
    igiari_unity_texture2d* tex = igiari_unity_texture2d_GetTexByName(array, count, tex_name);
    if (tex) {
        igiari_engine_texture* igiari_tex = igiari_engine_texture_LoadFromUnityAsset(tex);
        spr = igiari_engine_sprite_Create(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, igiari_tex);
    }
    
    igiari_unity_texture2d_FreeTexture2DArray(array, count);
    igiari_unity_bundle_Free(bundle);
    
    return spr;
}

igiari_audio_music* LoadMusicFromUnityBundle(char* bundle_name, char* node_name, char* clip_name) {
    igiari_audio_music* mus = NULL;
    
    igiari_unity_bundle* bundle = igiari_unity_bundle_Read(bundle_name);
    if (!bundle) return NULL;
    
    int count = 0;
    igiari_unity_audioclip** array = igiari_unity_audioclip_GetAllClipsFromNode(bundle, node_name, &count);
    if (!array || count == 0) {
        igiari_unity_bundle_Free(bundle);
        return NULL;
    }
    
    igiari_unity_audioclip* clip = igiari_unity_audioclip_GetClipByName(array, count, clip_name);
    if (clip) {
        mus = igiari_audio_CreateMusicFromAudioClip(clip);
    }
    
    // Free resources, but only if not already handled by another function
    igiari_unity_audioclip_FreeAudioClipArray(array, count);
    igiari_unity_bundle_Free(bundle);
    
    return mus;
}

igiari_audio_se* LoadSEFromUnityBundle(char* bundle_name, char* node_name, char* clip_name) {
    igiari_audio_se* se = NULL;
    
    igiari_unity_bundle* bundle = igiari_unity_bundle_Read(bundle_name);
    if (!bundle) return NULL;
    
    int count = 0;
    igiari_unity_audioclip** array = igiari_unity_audioclip_GetAllClipsFromNode(bundle, node_name, &count);
    if (!array || count == 0) {
        igiari_unity_bundle_Free(bundle);
        return NULL;
    }
    
    igiari_unity_audioclip* clip = igiari_unity_audioclip_GetClipByName(array, count, clip_name);
    if (clip) {
        se = igiari_audio_CreateSEFromAudioClip(clip);
    }

    igiari_unity_audioclip_FreeAudioClipArray(array, count);
    igiari_unity_bundle_Free(bundle);
    
    return se;
}

int main(int argc,char **argv) {
    igiari_engine_core_Initialize("igiari", 1280, 720, 1920, 1080, 60);
    
    igiari_imgui_CreateContext(IGIARI_ENGINE_SDLWINDOW, IGIARI_ENGINE_GLCONTEXT);
    ImFont* font = igiari_imgui_AddFontFromFileTTF("font.otf", 45.0f, NULL, NULL);
    
    igiari_engine_shader* shader = igiari_engine_shader_Create(IGIARI_SHADERS_SPRITE_VERT, IGIARI_SHADERS_SPRITE_FRAG);
    igiari_mdt mdt = igiari_mdt_Read("sc0_text_u.mdt.dec");
    
    igiari_engine_sprite* bg = LoadSpriteFromUnityBundle("games/trilogy/menu/common/title_back.unity3d", "CAB-ae9ba770903927e9f56b334635169106", "title_back");
    bg->x = 1920.0f / 2.0f; bg->y = 1080.0f / 2.0f;
    
    igiari_engine_sprite* talk_bg = LoadSpriteFromUnityBundle("games/trilogy/menu/common/talk_bg.unity3d", "CAB-9599caae236daa67f158ca412c6c6480", "talk_bg");
    talk_bg->x = 1920.0f / 2.0f; talk_bg->y = 1080.0f / 2.0f;
	
	igiari_engine_sprite* talk_bg_2 = LoadSpriteFromUnityBundle("games/trilogy/menu/common/menu_bg.unity3d", "CAB-328d4f82296c50ad6d8ed3ef246930c7", "menu_bg");
    talk_bg_2->tex_x_offset = 0.0f; talk_bg_2->tex_y_offset = 40.0f; talk_bg_2->tex_w_offset = 1920.0f; talk_bg_2->tex_h_offset = 40.0f;
    talk_bg_2->x = 1920.0f / 2.0f; talk_bg_2->y = 1080.0f / 2.0f; talk_bg_2->width = 1985; talk_bg_2->height = 40;
    
    igiari_engine_sprite* name_bg = LoadSpriteFromUnityBundle("games/trilogy/menu/common/name_bg.unity3d", "CAB-0716a8e7ded0114844f52a99c46c0dcf", "name_bg");
    name_bg->x = 1920.0f / 2.0f; name_bg->y = 1080.0f / 2.0f;

    igiari_trilogy_LoadSound(43, "games/trilogy/");
    igiari_trilogy_LoadBGM(23, "games/trilogy/");
    
    float textbox_y = 340;
    bool play = false;
    
    SDL_Event e;
    while(IGIARI_ENGINE_RUNNING)
    {
        igiari_engine_core_StartUpdate(&e);
        
        const Uint8* currentKeyboardState = SDL_GetKeyboardState(NULL);
        if (currentKeyboardState[SDL_SCANCODE_W] && !play) {
            play = true;
            igiari_trilogy_textbox_Init(mdt.operations, 200);
            //igiari_trilogy_effects_Perform(IGIARI_TRILOGY_EFFECTS_SHAKE);
        }
        
        igiari_trilogy_textbox_Update(1.0f / 60.0f);
        igiari_trilogy_effects_Update();

        igiari_engine_core_StartRender();
                talk_bg->y = (1080.0f / 2.0f) + textbox_y;
				talk_bg_2->x = (1920.0f / 2.0f) + 1400.0f; talk_bg_2->y = (1080.0f / 2.0f) + 119.0f + textbox_y;
                name_bg->x = (1920.0f / 2.0f) - 650.0f; name_bg->y = (1080.0f / 2.0f) + textbox_y - 133.0f;
                
                igiari_engine_sprite_Draw(talk_bg, shader);
				igiari_engine_sprite_Draw(talk_bg_2, shader);
                igiari_engine_sprite_Draw(name_bg, shader);
                
                float scale_x = ((float)IGIARI_ENGINE_VIEWPORT_WIDTH / (float)IGIARI_ENGINE_TARGETSIZE_WIDTH);
                float scale_y = ((float)IGIARI_ENGINE_VIEWPORT_HEIGHT / (float)IGIARI_ENGINE_TARGETSIZE_HEIGHT);
            
                igiari_imgui_NewFrame();
                
                igiari_imgui_SetNextWindowBgAlpha(0.0f);
                igiari_imgui_SetNextWindowPos(0.0f, 0.0f, 1 << 0, 0.0f, 0.0f);
                igiari_imgui_SetNextWindowSize(
                    (float)IGIARI_ENGINE_WINDOW_WIDTH,
                    (float)IGIARI_ENGINE_WINDOW_HEIGHT,
                    1 << 0
                );
                
                igiari_imgui_Begin("##text", NULL, igiari_imgui_WindowFlags_NoTitleBar | igiari_imgui_WindowFlags_NoResize | igiari_imgui_WindowFlags_NoScrollbar | igiari_imgui_WindowFlags_NoMove | igiari_imgui_WindowFlags_NoCollapse);
                //igiari_imgui_Begin("Phoenix", NULL, 0);
                    igiari_trilogy_textbox_Render(font);
                    igiari_trilogy_effects_Render();
                    
                    
                igiari_imgui_End();
                
                igiari_imgui_SetNextWindowBgAlpha(0.5f);
                igiari_imgui_SetNextWindowPos(
                        (10.0f) * scale_x + IGIARI_ENGINE_VIEWPORT_OFFSET_X,
                        (1070.0f) * scale_y + IGIARI_ENGINE_VIEWPORT_OFFSET_Y,
                        1 << 0,
                        0.0f, 1.0f
                    );
                igiari_imgui_Begin("FPS", NULL, igiari_imgui_WindowFlags_NoTitleBar|igiari_imgui_WindowFlags_NoResize|igiari_imgui_WindowFlags_AlwaysAutoResize|igiari_imgui_WindowFlags_NoMove|igiari_imgui_WindowFlags_NoSavedSettings|igiari_imgui_WindowFlags_NoFocusOnAppearing|igiari_imgui_WindowFlags_NoNav);
                    igiari_imgui_Text("FPS: %i", 0);
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