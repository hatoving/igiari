#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <vorbis/vorbisfile.h>

#include "audio.h"

#include "../assets/fmod/fsb.h"
#include "../assets/fmod/rebuild.h"

int igiari_audio_Initialize() {
	printf("[igiari, audio] Initing audio...\n");
	Mix_Init(MIX_INIT_OGG);
	
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		printf("[igiari, audio] Failed to initialize SDL_mixer.\n");
	}
	
	Mix_AllocateChannels(52);
	
	return 0;
}

igiari_audio_music* igiari_audio_CreateMusicFromAudioClip(igiari_unity_audioclip* clip) {
	int loop_start, loop_end, size;
	char* data = igiari_unity_audioclip_GetOggFileFromClip(clip, &size, &loop_start, &loop_end);
	return igiari_audio_CreateMusicFromData(data, size, ((float)loop_start / (float)clip->freq), ((float)loop_end / (float)clip->freq));
}

igiari_audio_music* igiari_audio_CreateMusicFromData(void* data, size_t len, float loop_start, float loop_end) {
	printf("[igiari, audio] Loading data... (%i bytes, %f-%f)\n", len, loop_start, loop_end);
	igiari_audio_music* igiari_music = (igiari_audio_music*)malloc(sizeof(igiari_audio_music));
	
	SDL_RWops* rw = SDL_RWFromMem(data, len);
	Mix_Music* sdl_music = Mix_LoadMUS_RW(rw, 1);
	
	igiari_music->src = sdl_music;
	igiari_music->loop_start = loop_start;
	igiari_music->loop_end = loop_end;
	
	return igiari_music;
}

void igiari_audio_PlayMusic(igiari_audio_music* music) {
	
}

void igiari_audio_Quit() {
	printf("[igiari, audio] Quitting audio...\n");
	Mix_CloseAudio();
}