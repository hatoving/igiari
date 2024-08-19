#ifndef IGIARI_ENGINE_AUDIO_H
#define IGIARI_ENGINE_AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>

extern ALCdevice* IGIARI_AUDIO_DEVICE;
extern ALCcontext* IGIARI_AUDIO_CONTEXT;

int igiari_audio_Initialize();
ALuint igiari_audio_LoadOggFile(char* ogg_data);
void igiari_audio_Quit();

#endif