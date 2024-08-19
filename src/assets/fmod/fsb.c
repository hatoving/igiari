#include "fsb.h"
#include "../../utils/reader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint32_t key;
    int value;
} igiari_fmod_fsb_freqtable;

const igiari_fmod_fsb_freqtable IGIARI_FMOD_FREQUENCIES[] = {
    { 1, 8000 },
    { 2, 11000 },
    { 3, 11025 },
    { 4, 16000 },
    { 5, 22050 },
    { 6, 24000 },
    { 7, 32000 },
    { 8, 44100 },
    { 9, 48000 },
    { 10, 96000 }
};

int igiari_fmod_fsb_GetFreq(int freq_id) {
    for (int i = 0; i < 9; i++)
    {
        if (IGIARI_FMOD_FREQUENCIES[i].key == freq_id) {
            return IGIARI_FMOD_FREQUENCIES[i].value;
        }
    }
    return -1;
}

igiari_fmod_fsb_sample_chunk igiari_fmod_fsb_impl_ReadChunkFromPtr(igiari_fmod_fsb_sample_metadata metadata, char* ptr, size_t* bytes_read) {
    igiari_fmod_fsb_sample_chunk chunk;

    uint32_t chunk_info_raw = igiari_utils_reader_ReadUInt24FromPtr(ptr); ptr += 3;
    chunk.more_chunks = chunk_info_raw & 0x01;
    chunk.chunk_size = chunk_info_raw >> 1;
    chunk.chunk_type = *(uint8_t*)ptr; ptr += 1;

    char* pos = ptr;
    //printf("more_chunks : %i\n", chunk.more_chunks);
    //printf("type : %i\n", chunk.chunk_type);
    switch (chunk.chunk_type)
    {
        case 0x16: // vorbis data
            chunk.chunk.vorbis = malloc(sizeof(igiari_fmod_chunk_vorbis));
            chunk.chunk.vorbis->crc32 = *(uint32_t*)ptr; ptr += 4;

            chunk.chunk.vorbis->data = malloc(chunk.chunk_size - 4);
            memcpy(chunk.chunk.vorbis->data, ptr, chunk.chunk_size - 4);

            ptr += chunk.chunk_size - 4;
            break;

        case 2: // freq data
            chunk.chunk.freq = malloc(sizeof(igiari_fmod_chunk_freq));
            chunk.chunk.freq->freq_id = *(uint32_t*)ptr; ptr += 4;
            break;

        case 1: // channel data
            chunk.chunk.channel = malloc(sizeof(igiari_fmod_chunk_channel));
            chunk.chunk.channel->num_channel = *(uint8_t*)ptr; ptr += 1;
            break;
        
        case 6: // loop data
            chunk.chunk.loop = malloc(sizeof(igiari_fmod_chunk_loop));
            chunk.chunk.loop->loop_start = *(uint32_t*)ptr; ptr += 4;
            chunk.chunk.loop->loop_end = *(uint32_t*)ptr; ptr += 4;
            printf("[igiari, fsb] Loop points: {%i -- %i}\n", chunk.chunk.loop->loop_start, chunk.chunk.loop->loop_end);
            break;

        case 7: // dsp data
            chunk.chunk.dsp = malloc(sizeof(igiari_fmod_chunk_dsp));

            chunk.chunk.dsp->metadata = &metadata;
            chunk.chunk.dsp->channel_data = malloc(metadata.channels * sizeof(short*));

            int channel_data_list_count = 0;
            for (int i = 0; i < metadata.channels; i++)
            {
                chunk.chunk.dsp->channel_data[i] = malloc(metadata.channels * sizeof(short));
                channel_data_list_count += 1;
            }
            
            for (int i = 0; i < metadata.channels; i++)
            {
                for (int i = 0; i < 16; i++)
                {
                    chunk.chunk.dsp->channel_data[i] = realloc(chunk.chunk.dsp->channel_data[i], (channel_data_list_count + 1) * sizeof(short));
                    chunk.chunk.dsp->channel_data[i][channel_data_list_count] = (igiari_utils_reader_ConvertUInt16_LEtoBE(*(uint16_t*)ptr)); ptr += 2;
                    channel_data_list_count += 1;
                }
                
                ptr += 14;
            }

            break;
        
        default:
            printf("[igiari, fsb] Found unk chunk type: %i. Size: %i\n", chunk.chunk_type, chunk.chunk_size);
            chunk.chunk.unk = malloc(sizeof(igiari_fmod_chunk_unk));
            chunk.chunk.unk->data = malloc(chunk.chunk_size);
            memcpy(chunk.chunk.unk->data, ptr, chunk.chunk_size);
            ptr += chunk.chunk_size;
            break;
    }
    int actual_bytes_read = ptr - pos;
    if (actual_bytes_read != chunk.chunk_size) {
        printf("YOU FUCKED UP.\n");
    }

    *bytes_read = actual_bytes_read;

    return chunk;
}

igiari_fmod_fsb_sample_metadata igiari_fmod_fsb_impl_ReadMetadataFromPtr(char* ptr) {
    igiari_fmod_fsb_sample_metadata metadata;

    uint64_t encoded = *(uint64_t*)ptr;
    ptr += 8;

    // Extract metadata using bit masking and shifting
    metadata.has_any_chunks = (encoded & 1) == 1;

    // Extract freq_id from bits 1 to 4
    metadata.freq_id = (encoded >> 1) & 0xF;

    // Extract pow2 from bits 5 to 6
    int pow2 = (encoded >> 5) & 0x3;
    metadata.num_channels = 1 << pow2;

    if (metadata.num_channels > 2) {
        printf("[igiari, fsb] WARNING: >2 channels not supported\n");
    } else {
        printf("[igiari, fsb] Channel count: %i\n", metadata.freq_id);
    }

    metadata.is_stereo = (metadata.num_channels == 2);

    // Extract data_offset from bits 7 to 33 and multiply by 32
    metadata.data_offset = ((encoded >> 7) & 0x7FFFFFF) * 32;
    //printf("DATA OFF: %i\n", metadata.data_offset);

    // Extract sample_count from bits 34 to 63
    metadata.sample_count = (encoded >> 34) & 0x3FFFFFFF;

    return metadata;
}

igiari_fmod_fsb_header igiari_fmod_fsb_impl_ReadHeaderFromPtr(char* ptr, size_t* bytes_read) {
    igiari_fmod_fsb_header header;

    char* pos = ptr;

    header.magic = malloc(5);
    memcpy(header.magic, ptr, 4); ptr += 4;
    header.magic[5] = '\0';

    if (strcmp(header.magic, "FSB5") < 0) {
        printf("[igiari, fsb] Invalid header. (%s)\n", header.magic);
    }

    header.version = *(uint32_t*)ptr; ptr += 4;
    header.num_samples = *(uint32_t*)ptr; ptr += 4;
    header.sample_headers_size = *(uint32_t*)ptr; ptr += 4;
    header.name_table_size = *(uint32_t*)ptr; ptr += 4;
    header.data_size = *(uint32_t*)ptr; ptr += 4;
    header.audio_type = *(uint32_t*)ptr; ptr += 4;

    ptr += 4; // should always be 0x1C

    printf("[igiari, fsb] Audio type: %i\n", header.audio_type);

    if (header.version == 0) {
        header.header_size = 0x40;
        ptr += 4;
    }
    else {
        header.header_size = 0x3C;
    }
    ptr += 4;

    uint64_t hash_lower = *(uint64_t*)ptr; ptr += 8;
    uint64_t hash_higher = *(uint64_t*)ptr; ptr += 8;

    ptr += 8;
    char* sample_header_start = ptr;

    header.samples = malloc(sizeof(igiari_fmod_fsb_sample_metadata));
    int sample_count = 0;
    for (int i = 0; i < header.num_samples; i++)
    {
        //printf("sample: %i\n", i);
        igiari_fmod_fsb_sample_metadata metadata = igiari_fmod_fsb_impl_ReadMetadataFromPtr(ptr); ptr += 8;

        if (metadata.has_any_chunks == 0) {
            header.samples = realloc(header.samples, (sample_count + 1) * sizeof(igiari_fmod_fsb_sample_metadata));
            header.samples[sample_count] = metadata;
            sample_count++;

            continue;
        }

        metadata.chunks = malloc(sizeof(igiari_fmod_fsb_sample_chunk));
        int chunk_count = 0;
        
        igiari_fmod_fsb_sample_chunk next_chunk;
        do
        {
            size_t bytes_read = 0;
            next_chunk = igiari_fmod_fsb_impl_ReadChunkFromPtr(metadata, ptr, &bytes_read);

            metadata.chunks = realloc(metadata.chunks, (chunk_count + 1) * sizeof(igiari_fmod_fsb_sample_chunk));
            metadata.chunks[chunk_count] = next_chunk;
            chunk_count++;

            ptr += bytes_read + 4;
        } while (next_chunk.more_chunks);
        metadata.chunk_count = chunk_count;

        for (int i = 0; i < chunk_count; i++)
        {
            if (metadata.chunks[i].chunk_type == 2) {
                metadata.freq_id = metadata.chunks[i].chunk.freq->freq_id;
            }
        }

        header.samples = realloc(header.samples, (sample_count + 1) * sizeof(igiari_fmod_fsb_sample_metadata));
        header.samples[sample_count] = metadata;
        sample_count++;
    }
    header.sample_count = sample_count;
    header.is_valid = 1;

    *bytes_read = (ptr - pos);
    return header;
}

igiari_fmod_fsb* igiari_fmod_fsb_ReadFromPtr(char* ptr) {
    igiari_fmod_fsb* fsb = (igiari_fmod_fsb*)malloc(sizeof(igiari_fmod_fsb));

    char* start = ptr;

    size_t bytes_read = 0;
    //printf("[igiari, fsb] pos: %p\n", ptr);
    fsb->header = igiari_fmod_fsb_impl_ReadHeaderFromPtr(ptr, &bytes_read); ptr += bytes_read;
    //printf("[igiari, fsb] pos: %p\n", ptr);

    fsb->samples = malloc(sizeof(igiari_fmod_fsb_sample));
    fsb->sample_count = 0;

    printf("[igiari, fsb] Reading %i samples\n", fsb->header.sample_count);
    for (int i = 0; i < fsb->header.sample_count; i++)
    {
        igiari_fmod_fsb_sample_metadata* metadata = &fsb->header.samples[i];

        int data_offset = metadata->data_offset;
        int size = fsb->header.data_size;

        if (i < fsb->header.sample_count - 1) {
            size = fsb->header.samples[i + 1].data_offset;
        }

        igiari_fmod_fsb_sample sample;
        sample.metadata = metadata;

        //ptr = start + first_byte_of_sample;
        //printf("%i == %i\n", last_byte_of_sample - first_byte_of_sample, fsb->header.data_size);
        ptr = start + fsb->header.header_size + fsb->header.name_table_size + fsb->header.sample_headers_size;
        //printf("offset: %i\n", fsb->header.data_size);

        sample.sample_bytes = malloc(fsb->header.data_size);
        memcpy(sample.sample_bytes, ptr, fsb->header.data_size); ptr += fsb->header.data_size;
        sample.sample_len = fsb->header.data_size;

        if (fsb->header.name_table_size > 0) {
            char* name_offset_offset = (char*)(fsb->header.header_size + fsb->header.sample_headers_size + 4 + i);
            ptr = name_offset_offset;

            char* name_offset = (char*)(*(uint32_t*)ptr); ptr += 4;
            name_offset += fsb->header.header_size + fsb->header.sample_headers_size;

            sample.name = igiari_utils_reader_ReadStringTilNull_FromPointer(ptr, &bytes_read); ptr += bytes_read;
            printf("[igiari, fsb] Sample name: %s\n", sample.name);
            break;
        }

        fsb->samples = realloc(fsb->samples, (fsb->sample_count + 1) * sizeof(igiari_fmod_fsb_sample));
        fsb->samples[fsb->sample_count] = sample;
        fsb->sample_count++;
    }

    return fsb;
}

