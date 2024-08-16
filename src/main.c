#include <string.h>

#include "engine/core.h"
#include "engine/object.h"
#include "engine/sprite.h"
#include "engine/unicode.h"

#include "mdt.h"

#include "unity/serialized_file.h"
#include "unity/bundle.h"

#include "unity/audio_clip.h"
#include "unity/texture2d.h"

#define RLUNICODE_IMPLEMENTATION 1
#include "rlunicode.h"

#include <malloc.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
    if (boshi_core_Initialize("igiari", 960, 544, 1280, 720, 60, true) != 0) {
        printf("[igiari] Failed to initiliate engine.\n");
        return 1;
    } else {
        boshi_core_SetRenderMode(boshi_RENDER_MODE_FIT);
    }

    igiari_mdt mdt = igiari_mdt_Read("ev0_mes_u.mdt.dec");

    igiari_unity_bundle* gs1_logo_bundle = igiari_unity_bundle_Read("titlegs1u.unity3d");
    int gs1_logo_tex_count = 0;

    igiari_unity_texture2d* gs1_logo_texarray = igiari_unity_texture2d_GetAllTexture2DsFromNode(gs1_logo_bundle, "CAB-1f36da66d6416727fb8d0b18cb649fae", &gs1_logo_tex_count);
    igiari_unity_texture2d* gs1_logo_tex = igiari_unity_texture2d_GetTexture2DByName(gs1_logo_texarray, gs1_logo_tex_count, "titleGS1u");
    Texture2D gs1_logo = igiari_unity_texture2d_ConvertIntoRaylibTexture2D(gs1_logo_tex);
    SetTextureFilter(gs1_logo, TEXTURE_FILTER_BILINEAR);

    free(gs1_logo_texarray);
    free(gs1_logo_bundle);

    igiari_unity_bundle* game_bg_bundle = igiari_unity_bundle_Read("title_back.unity3d");
    int game_bg_tex_count = 0;

    igiari_unity_texture2d* game_bg_texarray = igiari_unity_texture2d_GetAllTexture2DsFromNode(game_bg_bundle, "CAB-ae9ba770903927e9f56b334635169106", &game_bg_tex_count);
    igiari_unity_texture2d* game_bg_tex = igiari_unity_texture2d_GetTexture2DByName(game_bg_texarray, game_bg_tex_count, "title_back");
    Texture2D game_bg = igiari_unity_texture2d_ConvertIntoRaylibTexture2D(game_bg_tex);
    SetTextureFilter(game_bg, TEXTURE_FILTER_BILINEAR);

    free(game_bg_texarray);
    free(game_bg_bundle);

    igiari_unity_bundle* bgm002_bundle = igiari_unity_bundle_Read("bgm002.unity3d");
    int clip_count = 0;
    igiari_unity_audioclip* clips = igiari_unity_audioclip_GetAllAudioClipsFromNode(bgm002_bundle, "CAB-f57039f1f4db3b22350bb67f1a459d32", &clip_count);

    Font font = LoadFont("FOT-ModeMinBLargeStd-R.otf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    //free(tex_unity);

    while(!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) {
            ToggleBorderlessWindowed();
        }
        
        boshi_core_BeginDrawing();
            ClearBackground(BLACK);
            //DrawTextCodepoint(font, GetCodepointFromUTF16(&mdt.operations[20].text[1], &wordsProcessed), (Vector2){20, 20}, 20, WHITE);
            //DrawText(mdt.operations[274].text, 20, 20, 16, WHITE);
            DrawTexturePro(
                game_bg, (Rectangle){0, 0, game_bg.width, -game_bg.height},
                (Rectangle){0, 0, 1280, 720},
                (Vector2){0, 0}, 0.0, WHITE
            );
            DrawTexturePro(
                gs1_logo, (Rectangle){0, 0, gs1_logo.width, -gs1_logo.height},
                (Rectangle){1280 / 2, 720 / 2 - 50, gs1_logo.width / 1.5, gs1_logo.height / 1.5},
                (Vector2){(gs1_logo.width / 1.5) / 2, ( gs1_logo.height / 1.5) / 2}, 0.0, WHITE
            );
            DrawTextEx(font, mdt.operations[115].text, (Vector2){20, 20}, 30, 2, WHITE);
        boshi_core_EndDrawing();
    }
    //UnloadTexture(texture);+
    UnloadFont(font);
    boshi_core_Quit();
    return 0;
}