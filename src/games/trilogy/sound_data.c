#include "sound_data.h"

#include "../../assets/unity/bundle.h"
#include "../../assets/unity/audio_clip.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

igiari_trilogy_sedata* igiari_game_sounds[] = {
    &(igiari_trilogy_sedata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/se/wsd/se001.unity3d",
            .nodes = (char*[]){
                "CAB-5e58b92db9f8162feceb444d7595ebbd",
            },
            .clip_name = "se001"
        },
        .id = 43,
        .se = NULL
    }
};
int igiari_game_sounds_count = 1;

igiari_trilogy_bgmdata* igiari_game_bgms[] = {
    &(igiari_trilogy_bgmdata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/bgm/bgm200.unity3d",
            .nodes = (char*[]){
                "CAB-b51dcfc932ed9c0b84264effd46acc63",
            },
            .clip_name = "bgm200"
        },
        .id = 400,
        .bgm = NULL
    }
};
int igiari_game_bgms_count = 1;

void igiari_trilogy_LoadSound(int id, char* game_path) {
	igiari_trilogy_sedata* data = NULL;
	
	for(int i = 0; i < igiari_game_sounds_count; i++) {
		if (igiari_game_sounds[i]->id == id) {
			printf("[igiari, trilogy] Loading SE (%i)...\n", id);
			data = igiari_game_sounds[i];
		}
	}
	
	igiari_audio_se* se = NULL;
	
	size_t full_path_len = strlen(game_path) + strlen(data->path->bundle_path) + 1;
    char* full_path = (char*)malloc(full_path_len);
	
	strcpy(full_path, game_path);
    strcat(full_path, data->path->bundle_path);
	
    igiari_unity_bundle* bundle = igiari_unity_bundle_Read(full_path);
    if (!bundle) return;
    
    int count = 0;
    igiari_unity_audioclip** array = igiari_unity_audioclip_GetAllClipsFromNode(bundle, data->path->nodes[0], &count);
    if (!array || count == 0) {
        igiari_unity_bundle_Free(bundle);
        return;
    }
    
    igiari_unity_audioclip* clip = igiari_unity_audioclip_GetClipByName(array, count, data->path->clip_name);
    if (clip) {
        se = igiari_audio_CreateSEFromAudioClip(clip);
    }

    igiari_unity_audioclip_FreeAudioClipArray(array, count);
    igiari_unity_bundle_Free(bundle);
	
	data->se = se;
	free(full_path);
}

igiari_audio_se* igiari_trilogy_GetSound(int id) {
	for(int i = 0; i < igiari_game_sounds_count; i++) {
		if (igiari_game_sounds[i]->id == id) {
			return igiari_game_sounds[i]->se;
		}
	}
}

void igiari_trilogy_LoadBGM(int id, char* game_path) {
	igiari_trilogy_bgmdata* data = NULL;
	
	for(int i = 0; i < igiari_game_bgms_count; i++) {
		if (igiari_game_bgms[i]->id == id) {
			printf("[igiari, trilogy] Loading BGM (%i)...\n", id);
			data = igiari_game_bgms[i];
		}
	}
	
	igiari_audio_music* bgm = NULL;
	
	size_t full_path_len = strlen(game_path) + strlen(data->path->bundle_path) + 1;
    char* full_path = (char*)malloc(full_path_len);
	
	strcpy(full_path, game_path);
    strcat(full_path, data->path->bundle_path);
	
    igiari_unity_bundle* bundle = igiari_unity_bundle_Read(full_path);
    if (!bundle) return;
    
    int count = 0;
    igiari_unity_audioclip** array = igiari_unity_audioclip_GetAllClipsFromNode(bundle, data->path->nodes[0], &count);
    if (!array || count == 0) {
        igiari_unity_bundle_Free(bundle);
        return;
    }
    
    igiari_unity_audioclip* clip = igiari_unity_audioclip_GetClipByName(array, count, data->path->clip_name);
    if (clip) {
        bgm = igiari_audio_CreateMusicFromAudioClip(clip);
    }

    igiari_unity_audioclip_FreeAudioClipArray(array, count);
    igiari_unity_bundle_Free(bundle);
	
	data->bgm = bgm;
	free(full_path);
}

igiari_audio_music* igiari_trilogy_GetBGM(int id) {
	for(int i = 0; i < igiari_game_bgms_count; i++) {
		if (igiari_game_bgms[i]->id == id) {
			return igiari_game_bgms[i]->bgm;
		}
	}
}