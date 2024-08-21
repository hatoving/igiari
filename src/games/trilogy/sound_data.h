#ifndef IGIARI_TRILOGY_SOUND_DATA_H
#define IGIARI_TRILOGY_SOUND_DATA_H

#include "../../engine/audio.h"

typedef struct {
	char* bundle_path;
	char** nodes;
	char* clip_name;
} igiari_trilogy_unitypath;

typedef struct {
	igiari_trilogy_unitypath* path;
	int id;
	igiari_audio_se* se;
} igiari_trilogy_sedata;
typedef struct {
	igiari_trilogy_unitypath* path;
	int id;
	igiari_audio_music* bgm;
} igiari_trilogy_bgmdata;

extern igiari_trilogy_sedata* igiari_game_sounds[];
extern igiari_trilogy_bgmdata* igiari_game_bgms[];

extern int igiari_game_sounds_count;
extern int igiari_game_bgms_count;

void igiari_trilogy_LoadSound(int id, char* game_path);
igiari_audio_se* igiari_trilogy_GetSound(int id);

void igiari_trilogy_LoadBGM(int id, char* game_path);
igiari_audio_music* igiari_trilogy_GetBGM(int id);

#endif