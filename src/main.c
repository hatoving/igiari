#include <stdio.h>
#include <string.h>

#include "mdt.h"

#include "unity/serialized_file.h"
#include "unity/bundle.h"
#include "unity/audio_clip.h"
#include "unity/texture2d.h"

#include "fmod/rebuild.h"

#include <malloc.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
    igiari_unity_bundle* gs1_logo_bundle = igiari_unity_bundle_Read("titlegs1u.unity3d");
    int gs1_logo_tex_count = 0;

    igiari_unity_texture2d* gs1_logo_texarray = igiari_unity_texture2d_GetAllTexFromNode(gs1_logo_bundle, "CAB-1f36da66d6416727fb8d0b18cb649fae", &gs1_logo_tex_count);
    igiari_unity_texture2d* gs1_logo_tex = igiari_unity_texture2d_GetTexByName(gs1_logo_texarray, gs1_logo_tex_count, "titleGS1u");

    free(gs1_logo_texarray);
    free(gs1_logo_bundle);

    igiari_unity_bundle* game_bg_bundle = igiari_unity_bundle_Read("title_back.unity3d");
    int game_bg_tex_count = 0;

    igiari_unity_texture2d* game_bg_texarray = igiari_unity_texture2d_GetAllTexFromNode(game_bg_bundle, "CAB-ae9ba770903927e9f56b334635169106", &game_bg_tex_count);
    igiari_unity_texture2d* game_bg_tex = igiari_unity_texture2d_GetTexByName(game_bg_texarray, game_bg_tex_count, "title_back");

    free(game_bg_texarray);
    free(game_bg_bundle);

    igiari_unity_bundle* bgm002_bundle = igiari_unity_bundle_Read("bgm002.unity3d");

    int clip_count = 0;
    igiari_unity_audioclip* clips = igiari_unity_audioclip_GetAllClipsFromNode(bgm002_bundle, "CAB-f57039f1f4db3b22350bb67f1a459d32", &clip_count);
    igiari_unity_audioclip* bgm002 = igiari_unity_audioclip_GetClipByName(clips, clip_count, "bgm002");

    free(clips);

    char* ptr = bgm002->data;
    igiari_fmod_fsb fsb = igiari_fmod_fsb_ReadFromPtr(ptr);
    
    int ogg_size = 0;
    char* ogg_data = igiari_fmod_rebuild_vorbis_Convert(&fsb.samples[0], &ogg_size);
    
    FILE* file = fopen("bgm002.ogg", "wb");
    fwrite(ogg_data, ogg_size, 1, file);
    fclose(file);

    free(ogg_data);

    return 0;
}