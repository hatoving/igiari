#ifndef IGIARI_FMOD_FSB_H
#define IGIARI_FMOD_FSB_H

#include "chunk.h"
#include <stdint.h>

typedef struct {
    int chunk_type;
    int chunk_size;
    uint8_t more_chunks;

    union {
        igiari_fmod_chunk_channel* channel;
        igiari_fmod_chunk_dsp* dsp;
        igiari_fmod_chunk_freq* freq;
        igiari_fmod_chunk_loop* loop;
        igiari_fmod_chunk_unk* unk;
        igiari_fmod_chunk_vorbis* vorbis;
    } chunk;
    
} igiari_fmod_fsb_sample_chunk;

typedef struct {
    uint8_t has_any_chunks;

    int freq_id;
    uint64_t data_offset;

    int chunk_count;
    igiari_fmod_fsb_sample_chunk* chunks;
    int num_channels;

    uint8_t is_stereo;
    uint64_t sample_count;

    int freq;
    int channels;
} igiari_fmod_fsb_sample_metadata;

typedef struct {
    uint8_t is_valid;
    char* magic;

    int audio_type;
    int version;
    int num_samples;

    int header_size;
    int sample_headers_size;
    int name_table_size;
    int data_size;

    int sample_count;
    igiari_fmod_fsb_sample_metadata* samples;
} igiari_fmod_fsb_header;

typedef struct {
    igiari_fmod_fsb_sample_metadata* metadata;
    char* sample_bytes;
    int sample_len;
    char* name;
} igiari_fmod_fsb_sample;

typedef struct {
    igiari_fmod_fsb_header header;

    int sample_count;
    igiari_fmod_fsb_sample* samples;
} igiari_fmod_fsb;
igiari_fmod_fsb igiari_fmod_fsb_ReadFromPtr(char* ptr);

int igiari_fmod_fsb_GetFreq(int freq_id);
#endif
