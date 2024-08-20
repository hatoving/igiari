#include <stdio.h>
#include <string.h>

#include "../../utils/reader.h"
#include "texture2d.h"

igiari_unity_texture2d* igiari_unity_texture2d_ReadFromPtr(const char* ptr) {
    igiari_unity_texture2d* tex = (igiari_unity_texture2d*)malloc(sizeof(igiari_unity_texture2d));

    tex->forced_fallback_format = *(int32_t*)ptr; ptr += 4;
    tex->downscale_fallback = *(int8_t*)ptr; ptr += 1;

    uintptr_t addr = (uintptr_t)ptr;
    addr = (addr + 3) & ~((uintptr_t)3);
    ptr = (const char*)addr;

    tex->width = *(int32_t*)ptr; ptr += 4;
    tex->height = *(int32_t*)ptr; ptr += 4;

    tex->size = *(int32_t*)ptr; ptr += 4;

    tex->texture_format = *(int32_t*)ptr; ptr += 4;
    tex->mip_count = *(int32_t*)ptr; ptr += 4;

    tex->is_readable = *(uint8_t*)ptr; ptr += 1;

    addr = (uintptr_t)ptr;
    addr = (addr + 3) & ~((uintptr_t)3);
    ptr = (const char*)addr;

    tex->image_count = *(int32_t*)ptr; ptr += 4;
    tex->tex_dimension = *(int32_t*)ptr; ptr += 4;

    tex->texture_settings.filter_mode = *(int32_t*)ptr; ptr += 4;
    tex->texture_settings.aniso = *(int32_t*)ptr; ptr += 4;
    tex->texture_settings.mip_bias = *(float*)ptr; ptr += 4;

    tex->texture_settings.wrap_mode = *(int32_t*)ptr; ptr += 4;
    ptr += 8;

    tex->lightmap_format = *(int32_t*)ptr; ptr += 4;
    tex->colorspace = *(int32_t*)ptr; ptr += 4;
    tex->data_size = *(int32_t*)ptr; ptr += 4;

    tex->info.offset = *(uint32_t*)ptr; ptr += 4;
    tex->info.size = *(uint32_t*)ptr; ptr += 4;

    uint32_t string_len = *(uint32_t*)ptr; ptr += 4;

    tex->info.path = malloc(string_len + 1);
    memcpy(tex->info.path, ptr, string_len); ptr += string_len;
    tex->info.path[string_len] = '\0';
    //tex.info.path = igiari_utils_reader_ReadStringTilNull_FromPointer(ptr, &bytes_read); ptr += bytes_read;

    //printf("W/H: {%s}\n", tex.info.path);

    return tex;
}

igiari_unity_texture2d** igiari_unity_texture2d_GetAllTexFromNode(igiari_unity_bundle* bundle, char* path, int* tex_read) {
    //igiari_unity_serizaliedfile file =  igiari_unity_serizaliedfile_ReadFromData(bundle->uncompressed_data, igiari_unity_bundle_GetNodeByPath(&bundle, path)->offset);
    //CAB-1f36da66d6416727fb8d0b18cb649fae
    igiari_unity_texture2d** tex_array = (igiari_unity_texture2d**)malloc(sizeof(igiari_unity_texture2d*));
    int tex_count = 0;

    igiari_unity_serizaliedfile file = igiari_unity_serizaliedfile_ReadFromData(bundle->uncompressed_data, igiari_unity_bundle_GetNodeByPath(bundle, path)->offset);

    int object_count = 0;
    igiari_unity_objectinfo* object_infos = igiari_unity_serializedfile_GetAllObjectsOfType(&file, 28, &object_count); // 28 == Texture2D

    const char* starting_ptr = igiari_unity_bundle_GetNodeDataByPath(bundle, path);
    const char* ptr = 0;

    for (int i = 0; i < object_count; i++)
    {
        igiari_unity_texture2d* tex;

        ptr = starting_ptr + object_infos[i].byte_start;
        uint32_t string_len = *(uint32_t*)ptr; ptr += 4;

        char* name = (char*)malloc(string_len + 1);
        memcpy(name, ptr, string_len); ptr += string_len;
        name[string_len] = '\0';
        //printf("[igiari, unity, tex2d] \"name\": %s\n", name);

        uintptr_t addr = (uintptr_t)ptr;
        addr = (addr + 3) & ~((uintptr_t)3);
        ptr = (const char*)addr;

        tex = igiari_unity_texture2d_ReadFromPtr(ptr);
        ptr = bundle->uncompressed_data + (igiari_unity_bundle_GetNodeByPath(bundle, igiari_unity_bundle_impl_GetFileNameOfPath(tex->info.path))->offset + tex->info.offset);

        tex->data = malloc(tex->info.size);
        memcpy(tex->data, ptr, tex->info.size);

        tex->name = name;
        //printf("[igiari, unity, tex2d] data_size: %i\n", tex.info.size);

        tex_array = realloc(tex_array, (tex_count + 1) * sizeof(igiari_unity_texture2d*));
        tex_array[tex_count] = tex;
        tex_count++;
        
        free(name);
    }

    free(object_infos);

    *tex_read = tex_count;
    return tex_array;
}

igiari_unity_texture2d* igiari_unity_texture2d_GetTexByName(igiari_unity_texture2d** array, int size, char* name) {
    for (int i = 0; i < size; i++) {
        if (strcmp(array[i]->name, name) >= 0) {
            printf("[igiari, unity, tex2d] Found \"%s\"\n", array[i]->name);
            return array[i];
        }
    }
    printf("[igiari, unity, tex2d] Didn't find \"%s\"\n", name);
    return NULL;
}

void igiari_unity_texture2d_FreeTexture2D(igiari_unity_texture2d* tex) {
    if (tex) {
        if (tex->data) {
            free(tex->data);
            tex->data = NULL;
        }
        tex->name = "";
        tex->ptr = NULL;
        free(tex);
    }
}

void igiari_unity_texture2d_FreeTexture2DArray(igiari_unity_texture2d** array, int size) {
    if (array) {
        for (int i = 0; i < size; i++) {
            if (array[i]) {
                igiari_unity_texture2d_FreeTexture2D(array[i]);
                array[i] = NULL;
            }
        }
        free(array);
    }
}

/*Texture2D igiari_unity_texture2d_ConvertIntoRaylib(igiari_unity_texture2d* tex) {
    int format = 0;
    switch(tex->>texture_format) {
        case 3: { // RGB24
            format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
            break;
        }
        case 4: { // RGB3
            format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
            break;
        }
        case 10: { //DXT1
            format = PIXELFORMAT_COMPRESSED_DXT1_RGBA;
            break;
        }
    }

    Image image = {
        0,
        (int)tex->>width,
        (int)tex->>height,
        tex->>mip_count,
        format
    };
    image.data = RL_MALLOC(tex->>info.size);
    memcpy(image.data, tex->>data, tex->>info.size);

    Texture2D ray_tex = LoadTextureFromImage(image);

    UnloadImage(image);
    return ray_tex;
}*/