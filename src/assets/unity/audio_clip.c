#include "audio_clip.h"

#include "../fmod/fsb.h"
#include "../fmod/rebuild.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

igiari_unity_audioclip* igiari_unity_audioclip_ReadFromPtr(igiari_unity_bundle* bundle, const char* ptr) {
    igiari_unity_audioclip* clip = (igiari_unity_audioclip*)malloc(sizeof(igiari_unity_audioclip));

    if (bundle->header.unity_ver->major >= 5) {
        clip->load_type = *(int32_t*)ptr; ptr += 4;
        clip->channels = *(int32_t*)ptr; ptr += 4;
        clip->freq = *(int32_t*)ptr; ptr += 4;
        clip->bits_per_sample = *(int32_t*)ptr; ptr += 4;
        clip->length = *(float*)ptr; ptr += 4;

        clip->is_tracker_format = *(uint8_t*)ptr; ptr += 1;

        uintptr_t addr = (uintptr_t)ptr;
        addr = (addr + 3) & ~((uintptr_t)3);
        ptr = (const char*)addr;

        clip->subsound_index = *(uint32_t*)ptr; ptr += 4;
        clip->preload_audio_data = *(uint8_t*)ptr; ptr += 1;
        clip->load_in_background = *(uint8_t*)ptr; ptr += 1;
        clip->legacy3d = *(uint8_t*)ptr; ptr += 1;

        addr = (uintptr_t)ptr;
        addr = (addr + 3) & ~((uintptr_t)3);
        ptr = (const char*)addr;

        uint32_t string_len = *(uint32_t*)ptr; ptr += 4;

        clip->info.path = malloc(string_len + 1);
        memcpy(clip->info.path, ptr, string_len); ptr += string_len;
        clip->info.path[string_len] = '\0';

        addr = (uintptr_t)ptr;
        addr = (addr + 3) & ~((uintptr_t)3);
        ptr = (const char*)addr;

        clip->info.offset = *(uint64_t*)ptr; ptr += 8;
        clip->info.size = *(uint64_t*)ptr; ptr += 8;

        clip->compression_format = *(uint32_t*)ptr; ptr += 4;
    }

    return clip;
}

igiari_unity_audioclip** igiari_unity_audioclip_GetAllClipsFromNode(igiari_unity_bundle* bundle, char* path, int* clips_read) {
    igiari_unity_audioclip** clip_array = malloc(sizeof(igiari_unity_audioclip*));
    int clip_count = 0;

    igiari_unity_serizaliedfile file = igiari_unity_serizaliedfile_ReadFromData(bundle->uncompressed_data, igiari_unity_bundle_GetNodeByPath(bundle, path)->offset);

    int object_count = 0;
    igiari_unity_objectinfo* object_infos = igiari_unity_serializedfile_GetAllObjectsOfType(&file, 83, &object_count); // 83 == AudioClip

    const char* starting_ptr = igiari_unity_bundle_GetNodeDataByPath(bundle, path);
    const char* ptr = 0;

    for (int i = 0; i < object_count; i++)
    {
        igiari_unity_audioclip* clip = (igiari_unity_audioclip*)malloc(sizeof(igiari_unity_audioclip));

        ptr = starting_ptr + object_infos[i].byte_start;
        uint32_t string_len = *(uint32_t*)ptr; ptr += 4;

        char* name = (char*)malloc(string_len + 1);
        memcpy(name, ptr, string_len); ptr += string_len;
        name[string_len] = '\0';
       // printf("[igiari, unity, audclp] name: %s\n", name);

        uintptr_t addr = (uintptr_t)ptr;
        addr = (addr + 3) & ~((uintptr_t)3);
        ptr = (const char*)addr;

        clip = igiari_unity_audioclip_ReadFromPtr(bundle, ptr);

        //printf("path: %s\n", clip.info.path);

        ptr = bundle->uncompressed_data + (igiari_unity_bundle_GetNodeByPath(bundle, igiari_unity_bundle_impl_GetFileNameOfPath(clip->info.path))->offset + clip->info.offset);

        clip->data = malloc(clip->info.size);
        memcpy(clip->data, ptr, clip->info.size);

        clip->name = name;

        clip_array = realloc(clip_array, (clip_count + 1) * sizeof(igiari_unity_audioclip*));
        clip_array[clip_count] = clip;
        clip_count++;
    }

    *clips_read = clip_count;
    return clip_array;
}

igiari_unity_audioclip* igiari_unity_audioclip_GetClipByName(igiari_unity_audioclip** array, int size, char* name) {
    for (int i = 0; i < size; i++) {
        if (strcmp(array[i]->name, name) >= 0) {
            printf("[igiari, unity, tex2d] Found \"%s\"\n", array[i]->name);
            return array[i];
        }
    }
    printf("[igiari, unity, tex2d] Didn't find \"%s\"\n", name);
    return NULL;
}

char* igiari_unity_audioclip_GetOggFileFromClip(igiari_unity_audioclip* clip, int* size, int* loop_start, int* loop_end) {
    char* ptr = clip->data;
    igiari_fmod_fsb* fsb = igiari_fmod_fsb_ReadFromPtr(ptr);
    
    int ogg_size = 0;
    char* ogg_data = igiari_fmod_rebuild_vorbis_Convert(fsb->samples[0], &ogg_size, loop_start, loop_end);
    
    igiari_fmod_fsb_Free(fsb);
    *size = ogg_size;
    
    return ogg_data;
}

void igiari_unity_texture2d_FreeAudioClip(igiari_unity_audioclip* clip) {
    free(clip->data);
    free(clip->name);
    free(clip);
}

void igiari_unity_texture2d_FreeAudioClipArray(igiari_unity_audioclip** array, int size) {
    for(int i = 0; i < size; i++) {
        igiari_unity_texture2d_FreeAudioClip(array[i]);
    }
    free(array);
}