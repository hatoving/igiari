#include "sound_data.h"

#include "../../assets/unity/bundle.h"
#include "../../assets/unity/audio_clip.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

igiari_trilogy_sedata* igiari_game_sounds[] = {
	&(igiari_trilogy_sedata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/se/wsd/se000.unity3d",
            .nodes = (char*[]){
                "CAB-6568a26b80a5a01ccf87125eb854b8cf",
            },
            .clip_name = "se000"
        },
        .id = 42,
        .se = NULL
    },
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
    },
	&(igiari_trilogy_sedata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/se/wsd/se019.unity3d",
            .nodes = (char*[]){
                "CAB-93bf8285ba5efc0ec86e6247f0ff5282",
            },
            .clip_name = "se019"
        },
        .id = 67,
        .se = NULL
    },
	&(igiari_trilogy_sedata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/se/wsd/se054.unity3d",
            .nodes = (char*[]){
                "CAB-661f674db6eb0a79187da838af8a5cf3",
            },
            .clip_name = "se054"
        },
        .id = 126,
        .se = NULL
    },
	&(igiari_trilogy_sedata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/se/wsd/se090.unity3d",
            .nodes = (char*[]){
                "CAB-81c2050381d4fa968fcc75f9b8690fe1",
            },
            .clip_name = "se090"
        },
        .id = 350,
        .se = NULL
    }
};
int igiari_game_sounds_count = 5;

igiari_trilogy_bgmdata* igiari_game_bgms[] = {
	&(igiari_trilogy_bgmdata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/bgm/bgm000.unity3d",
            .nodes = (char*[]){
                "CAB-4fc2c3785f7f5499124295eab202a802",
            },
            .clip_name = "bgm000"
        },
        .id = 0,
        .bgm = NULL
    },
	&(igiari_trilogy_bgmdata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/bgm/bgm001.unity3d",
            .nodes = (char*[]){
                "CAB-b373211f73403620b7b0dd2863a6f202",
            },
            .clip_name = "bgm001"
        },
        .id = 1,
        .bgm = NULL
    },
	&(igiari_trilogy_bgmdata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/bgm/bgm002.unity3d",
            .nodes = (char*[]){
                "CAB-f57039f1f4db3b22350bb67f1a459d32",
            },
            .clip_name = "bgm002"
        },
        .id = 2,
        .bgm = NULL
    },
	&(igiari_trilogy_bgmdata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/bgm/bgm003.unity3d",
            .nodes = (char*[]){
                "CAB-999f6667abb7d52f9cab89a68f3f8d2a",
            },
            .clip_name = "bgm003"
        },
        .id = 3,
        .bgm = NULL
    },
	&(igiari_trilogy_bgmdata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/bgm/bgm004.unity3d",
            .nodes = (char*[]){
                "CAB-520eb4da4bfc3bfa57ac4ed5ab0e2315",
            },
            .clip_name = "bgm004"
        },
        .id = 4,
        .bgm = NULL
    },
	&(igiari_trilogy_bgmdata){
        .path = &(igiari_trilogy_unitypath){
            .bundle_path = "Sound/bgm/bgm023.unity3d",
            .nodes = (char*[]){
                "CAB-ddb8ae1681ba3c19a787bde9a00d6278",
            },
            .clip_name = "bgm023"
        },
        .id = 23,
        .bgm = NULL
    },
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
int igiari_game_bgms_count = 7;

void igiari_trilogy_LoadSound(int id, char* game_path) {
	printf("[igiari, trilogy] Loading SE (%i)...\n", id);
	igiari_trilogy_sedata* data = NULL;
	
	for(int i = 0; i < igiari_game_sounds_count; i++) {
		if (igiari_game_sounds[i]->id == id) {
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
	printf("[igiari, trilogy] Loading BGM (%i)...\n", id);
	igiari_trilogy_bgmdata* data = NULL;
	
	for(int i = 0; i < igiari_game_bgms_count; i++) {
		if (igiari_game_bgms[i]->id == id) {
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