#include <stdio.h>
#include <stdlib.h>

#include <vorbis/vorbisfile.h>
#include "audio.h"

ALCdevice* IGIARI_AUDIO_DEVICE = NULL;
ALCcontext* IGIARI_AUDIO_CONTEXT = NULL;

int igiari_audio_Initialize() {
	printf("[igiari, audio] Initing audio...\n");
	
	IGIARI_AUDIO_DEVICE = alcOpenDevice(NULL);
	if (!IGIARI_AUDIO_DEVICE) {
		fprintf(stderr, "[igiari, audio] Failed to open OpenAL device.\n");
		return -1;
	}
	
	IGIARI_AUDIO_CONTEXT = alcCreateContext(IGIARI_AUDIO_DEVICE, NULL);
	if (!IGIARI_AUDIO_CONTEXT) {
		fprintf(stderr, "[igiari, audio] Failed to create OpenAL context.\n");
		alcCloseDevice(IGIARI_AUDIO_DEVICE);
		return -1;
	}
	
	alcMakeContextCurrent(IGIARI_AUDIO_CONTEXT);
	return 0;
}

ALuint igiari_audio_LoadOggFile(char* ogg_data) {
	ALuint source;
	alGenSources((ALuint)1,  &source);
	
	alSourcei(source, AL_LOOPING, AL_TRUE);
	
	
	
	return source;
}

void igiari_audio_Quit() {
	printf("[igiari, audio] Quitting audio...\n");
	
	alcMakeContextCurrent(NULL);
    alcDestroyContext(IGIARI_AUDIO_CONTEXT);
    alcCloseDevice(IGIARI_AUDIO_DEVICE);
}