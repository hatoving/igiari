#ifndef IGIARI_FMOD_CHUNK_H
#define IGIARI_FMOD_CHUNK_H

#include "fsb.h"
#include <stdint.h>

typedef struct {
    uint8_t num_channel;
} igiari_fmod_chunk_channel;
typedef struct {
    short** channel_data;
    void* metadata;
} igiari_fmod_chunk_dsp;
typedef struct {
    uint32_t freq_id;
} igiari_fmod_chunk_freq;
typedef struct {
    char* data;
} igiari_fmod_chunk_unk;
typedef struct {
    uint32_t loop_start;
    uint32_t loop_end;
} igiari_fmod_chunk_loop;
typedef struct {
    uint32_t crc32;
    char* data;
} igiari_fmod_chunk_vorbis;

#endif
