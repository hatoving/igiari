#ifndef IGIARI_UNITY_AUDIO_CLIP_H
#define IGIARI_UNITY_AUDIO_CLIP_H

#include "bundle.h"
#include "object.h"

#include <stdint.h>

typedef struct {
    int load_type;
    int channels;
    int bits_per_sample;
    uint8_t is_tracker_format;
    uint8_t ambisonic;
    int flags;
    int subsound_index;
    uint8_t preload_audio_data;
    uint8_t load_in_background;

    int compression_format;
    uint8_t decompress_on_load;

    int format;
    int type;

    float length;
    int freq;
    int size;

    uint8_t legacy3d;
    uint8_t use_hardware;

    char* data;
    char* name;

    igiari_unity_object_streaminginfo info;
} igiari_unity_audioclip;
igiari_unity_audioclip* igiari_unity_audioclip_ReadFromPtr(igiari_unity_bundle* bundle, const char* ptr);

igiari_unity_audioclip** igiari_unity_audioclip_GetAllClipsFromNode(igiari_unity_bundle* bundle, char* path, int* tex_read);
igiari_unity_audioclip* igiari_unity_audioclip_GetClipByName(igiari_unity_audioclip** array, int size, char* name);

char* igiari_unity_audioclip_GetOggFileFromClip(igiari_unity_audioclip* clip, int* size, int* loop_start, int* loop_end);
//Music igiari_unity_audioclip_ConvertIntoRaylib(igiari_unity_audioclip* clip);
#endif