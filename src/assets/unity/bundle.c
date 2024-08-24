#include "bundle.h"

#include "../../utils/lz4.h"
#include "../../utils/reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* igiari_unity_bundle_impl_GetFileNameOfPath(char* path) {
    char* last_slash = strrchr(path, '/');

    if (last_slash == NULL) {
        return path;
    }
    
    return last_slash + 1;
}

igiari_unity_version* split_version(const char *version) {
    igiari_unity_version* ver = malloc(sizeof(igiari_unity_version));
    if (!ver) return NULL; // Check malloc success

    ver->major = ver->minor = ver->revision = ver->type_num = 0;
    ver->type_char = 'f';

    if (sscanf(version, "%i.%i.%i%c%i", &ver->major, &ver->minor, &ver->revision, &ver->type_char, &ver->type_num) == 5) {
        return ver;
    }

    if (sscanf(version, "%i.%i.%i", &ver->major, &ver->minor, &ver->revision) == 3) {
        return ver;
    }

    return ver;
}

igiari_unity_bundle* igiari_unity_bundle_Read(char* path) {
    igiari_unity_bundle* bundle = malloc(sizeof(igiari_unity_bundle));
    FILE* bundle_file = fopen(path, "rb");
    
    fseek(bundle_file, 0, SEEK_END);
    long file_size = ftell(bundle_file);
    fseek(bundle_file, 0, SEEK_SET);

    bundle->header.signature = igiari_utils_reader_ReadStringTilNull(bundle_file);
    bundle->header.version = igiari_utils_reader_ReadUInt32_BE(bundle_file);

    bundle->header.unity_rev = igiari_utils_reader_ReadStringTilNull(bundle_file);
    char* ver = igiari_utils_reader_ReadStringTilNull(bundle_file);

    //bundle->header.unity_ver = malloc(sizeof(igiari_unity_version));
    bundle->header.unity_ver = split_version(ver);
    //printf("[igiari, unity, bundle] Loading bundle (\"%s\", %i.%i.%i%c%i)\n", path, bundle->header.unity_ver->major, bundle->header.unity_ver->minor, bundle->header.unity_ver->revision, bundle->header.unity_ver->type_char, bundle->header.unity_ver->type_num);

    //printf("[igiari, unity, bundle] Unity version: (%i.%i.%i%c%i)\n", ver, bundle->header.unity_ver->major, bundle->header.unity_ver->minor, bundle->header.unity_ver->revision, bundle->header.unity_ver->type_char, bundle->header.unity_ver->type_num);

    bundle->header.size = (int64_t)igiari_utils_reader_ReadUInt64_BE(bundle_file);
    bundle->header.compressed_blocks_info_size = igiari_utils_reader_ReadUInt32_BE(bundle_file);
    bundle->header.uncompressed_blocks_info_size = igiari_utils_reader_ReadUInt32_BE(bundle_file);
    bundle->header.archive_flags = igiari_utils_reader_ReadUInt32_BE(bundle_file);

    char* block_info_bytes = (char*)malloc(bundle->header.compressed_blocks_info_size);

    if ((bundle->header.archive_flags & 0x80) != 0) {
        printf("[igiari, unity, bundle] Block info is at the end.\n");
        long pos = ftell(bundle_file);
        fseek(bundle_file, file_size - bundle->header.compressed_blocks_info_size, SEEK_SET);
        fread(block_info_bytes, bundle->header.compressed_blocks_info_size, 1, bundle_file);
        fseek(bundle_file, pos, SEEK_SET);
    } else {
        fread(block_info_bytes, bundle->header.compressed_blocks_info_size, 1, bundle_file);
    }

    int compression_type = (bundle->header.archive_flags & 0x3f);
    //printf("[igiari, unity, bundle] Compression type: %i\n", compression_type);

    unsigned char* uncompressed_block_info_bytes = (char*)malloc(bundle->header.uncompressed_blocks_info_size);

    switch (compression_type) {
        case 3: { //Lz4HC
            int result = LZ4_decompress_safe(block_info_bytes, uncompressed_block_info_bytes, bundle->header.compressed_blocks_info_size, bundle->header.uncompressed_blocks_info_size);
            if (result != bundle->header.uncompressed_blocks_info_size) {
                printf("[igiari, unity, bundle] LZ4: Failed to decompress data. (%i != %i)\n", result, bundle->header.uncompressed_blocks_info_size);
                free(block_info_bytes);
                free(uncompressed_block_info_bytes);
                return bundle;
            } else {
                //printf("[igiari, unity, bundle] LZ4: Succesfully decompressed data. %i\n", result);
            }
            free(block_info_bytes);
            break;
        }
    }

    const char* data_ptr = uncompressed_block_info_bytes;

    data_ptr += 16;
    int32_t block_info_count = igiari_utils_reader_ConvertUInt32_LEtoBE((*(int32_t*)data_ptr)); data_ptr += 4;

    //printf("[igiari, unity, bundle] Block info count: %i\n", block_info_count);

    bundle->storage_blocks = malloc(block_info_count * sizeof(igiari_unity_bundle_storageblock));
    int storage_block_count = 0;

    while (storage_block_count < block_info_count) {
        igiari_unity_bundle_storageblock dummy;

        dummy.uncompressed_size = igiari_utils_reader_ConvertUInt32_LEtoBE((*(uint32_t*)data_ptr)); data_ptr += 4;
        dummy.compressed_size = igiari_utils_reader_ConvertUInt32_LEtoBE((*(uint32_t*)data_ptr)); data_ptr += 4;
        dummy.flags = igiari_utils_reader_ConvertUInt16_LEtoBE((*(uint16_t*)data_ptr)); data_ptr += 2;

        //printf("[igiari, unity, bundle] block: {%i, %i, %i}\n", dummy.uncompressed_size, dummy.compressed_size, dummy.flags);

        bundle->storage_blocks[storage_block_count] = dummy;
        storage_block_count++;
    }

    int32_t node_count = igiari_utils_reader_ConvertUInt32_LEtoBE((*(int32_t*)data_ptr)); data_ptr += 4;
    //printf("[igiari, unity, bundle] Node count: %i\n", node_count);

    bundle->directory_info = (igiari_unity_bundle_node**)malloc(node_count * sizeof(igiari_unity_bundle_node*));
    int dir_info_count = 0;

    bundle->directory_info_size = node_count;
    while (dir_info_count < node_count) {
        igiari_unity_bundle_node* dummy = (igiari_unity_bundle_node*)malloc(sizeof(igiari_unity_bundle_node));

        dummy->offset = igiari_utils_reader_ConvertUInt64_LEtoBE((*(int64_t*)data_ptr)); data_ptr += 8;
        dummy->size = igiari_utils_reader_ConvertUInt32_LEtoBE((*(int64_t*)data_ptr)); data_ptr += 8;
        dummy->flags = igiari_utils_reader_ConvertUInt16_LEtoBE((*(uint32_t*)data_ptr)); data_ptr += 4;

        size_t bytes = 0;
        dummy->path = igiari_utils_reader_ReadStringTilNull_FromPointer(data_ptr, &bytes); data_ptr += bytes;
        //printf("[igiari, unity, bundle] Bundle path: %s, size: %i\n", dummy->path, bytes);

        bundle->directory_info[dir_info_count] = dummy;
        dir_info_count++;
    }

    free(uncompressed_block_info_bytes);

    data_ptr = 0x00;
    
    unsigned char* combined_block_data = (char*)malloc(1);
    int combined_block_data_count = 0;

    for (int i = 0; i < block_info_count; i++)
    {
        //printf("[igiari, unity, bundle] comp type: %i\n", bundle->storage_blocks[i].flags & 0x3F);
        switch(bundle->storage_blocks[i].flags & 0x3F) {
            case 0: {
                char* block_data = (char*)malloc(bundle->storage_blocks[i].compressed_size);
                fread(block_data, bundle->storage_blocks[i].compressed_size, 1, bundle_file);

                combined_block_data = (char *)realloc(combined_block_data, combined_block_data_count + bundle->storage_blocks[i].uncompressed_size);
                memcpy(combined_block_data + combined_block_data_count, block_data, bundle->storage_blocks[i].uncompressed_size);
                combined_block_data_count += bundle->storage_blocks[i].uncompressed_size;

                free(block_data);
                //
                break;
            }
            case 3: { //Lz4HC
                char* compressed_data = (char*)malloc(bundle->storage_blocks[i].compressed_size);
                fread(compressed_data, bundle->storage_blocks[i].compressed_size, 1, bundle_file);

                char* uncompressed_data = (char*)malloc(bundle->storage_blocks[i].uncompressed_size);

                int result = LZ4_decompress_safe(compressed_data, uncompressed_data, bundle->storage_blocks[i].compressed_size, bundle->storage_blocks[i].uncompressed_size);
                if (result != bundle->storage_blocks[i].uncompressed_size) {
                    printf("[igiari, unity, bundle] LZ4: Failed to decompress data. (%i != %i)\n", result, bundle->storage_blocks[i].uncompressed_size);
                    free(compressed_data);
                    free(uncompressed_data);
                    return bundle;
                }
                combined_block_data = (char *)realloc(combined_block_data, combined_block_data_count + bundle->storage_blocks[i].uncompressed_size);
                memcpy(combined_block_data + combined_block_data_count, uncompressed_data, bundle->storage_blocks[i].uncompressed_size);
                combined_block_data_count += bundle->storage_blocks[i].uncompressed_size;
                // append compressed_data to combined_block_data here
                //combined_block_data_count += bundle->storage_blocks[i].uncompressed_size;
                free(compressed_data);
                free(uncompressed_data);
                break;
            }
            default:
                printf("[igiari, unity, bundle] Found unsupported compression mode: %i\n", (bundle->storage_blocks[i].flags & 0x3F));
                break;
        }
    }

    //printf("[igiari, unity, bundle] final size: %i\n", combined_block_data_count);
    bundle->uncompressed_data_len = combined_block_data_count;
    bundle->uncompressed_data = combined_block_data;
    fclose(bundle_file);
    return bundle;
}

void igiari_unity_bundle_Free(igiari_unity_bundle* bundle) {
    if (bundle == NULL) return;

    // Free uncompressed data
    if (bundle->uncompressed_data) {
        free(bundle->uncompressed_data);
        bundle->uncompressed_data = NULL;  // Clear the pointer to avoid double-free
    }

    // Free directory info entries
    if (bundle->directory_info) {
        for (int i = 0; i < bundle->directory_info_size; i++) {
            if (bundle->directory_info[i]) {
                free(bundle->directory_info[i]);
                bundle->directory_info[i] = NULL;
            }
        }
        free(bundle->directory_info);
        bundle->directory_info = NULL;
    }

    // Free storage blocks
    if (bundle->storage_blocks) {
        free(bundle->storage_blocks);
        bundle->storage_blocks = NULL;
    }

    // Free the bundle itself
    free(bundle);
}

igiari_unity_bundle_node* igiari_unity_bundle_GetNodeByPath(igiari_unity_bundle* bundle, char* path) {
    for (int i = 0; i < bundle->directory_info_size; i++) {
        if (strcmp(bundle->directory_info[i]->path, path) >= 0) {
            //printf("[igiari, unity, bundle] Found bundle: %s\n", bundle->directory_info[i].path);
            return bundle->directory_info[i];
        } else {
            //printf("[igiari, unity, bundle] (%s [%i] != %s [%i]])\n", bundle->directory_info[i]->path, strlen(bundle->directory_info[i]->path), path, strlen(path));
        }
    }
    printf("[igiari, unity, bundle] Did not find bundle: \"%s\"\n", path);
    return NULL;
}

unsigned char* igiari_unity_bundle_GetNodeDataByPath(igiari_unity_bundle* bundle, char* path) {
    igiari_unity_bundle_node* node = igiari_unity_bundle_GetNodeByPath(bundle, path);
    unsigned char* data = malloc((bundle->uncompressed_data_len - node->offset));
    const char* ptr = bundle->uncompressed_data + node->offset;
    memcpy(data, ptr, (bundle->uncompressed_data_len - bundle->directory_info[1]->offset));

    return data;
}