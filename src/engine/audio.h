#ifndef IGIARI_ENGINE_AUDIO_H
#define IGIARI_ENGINE_AUDIO_H

#include "../assets/unity/audio_clip.h"
#include <SDL2/SDL_mixer_ext.h>

typedef struct {
	Mix_Music* src;
	float loop_start;
	float loop_end;
} igiari_audio_music;

int igiari_audio_Initialize();

igiari_audio_music* igiari_audio_CreateMusicFromData(void* data, size_t len, float loop_start, float loop_end);
igiari_audio_music* igiari_audio_CreateMusicFromAudioClip(igiari_unity_audioclip* clip);

void igiari_audio_Quit();

#endif