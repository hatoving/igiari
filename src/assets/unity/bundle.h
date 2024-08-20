#ifndef IGIARI_UNITY_BUNDLE_H
#define IGIARI_UNITY_BUNDLE_H

#include <stdint.h>

typedef struct {
    int major;
    int minor;
    int revision;
    char type_char;
    int type_num;
} igiari_unity_version;

typedef struct {
    char* signature;
    uint32_t version;

    igiari_unity_version* unity_ver;
    char* unity_rev;

    uint64_t size;

    uint32_t compressed_blocks_info_size;
    uint32_t uncompressed_blocks_info_size;

    int archive_flags;
} igiari_unity_bundle_header;

typedef struct {
    uint32_t uncompressed_size;
    uint32_t compressed_size;
    uint16_t flags;
} igiari_unity_bundle_storageblock;

typedef struct {
    uint64_t offset;
    uint64_t size;
    uint32_t flags;
    char* path;
} igiari_unity_bundle_node;

typedef struct {
    char* original_path;
    igiari_unity_bundle_header header;

    igiari_unity_bundle_storageblock* storage_blocks;

    int directory_info_size;
    igiari_unity_bundle_node** directory_info;

    int uncompressed_data_len;
    unsigned char* uncompressed_data;
} igiari_unity_bundle;
igiari_unity_bundle* igiari_unity_bundle_Read(char* path);
void igiari_unity_bundle_Free(igiari_unity_bundle* bundle);

unsigned char* igiari_unity_bundle_GetNodeDataByPath(igiari_unity_bundle* bundle, char* path);
igiari_unity_bundle_node* igiari_unity_bundle_GetNodeByPath(igiari_unity_bundle* bundle, char* path);

char* igiari_unity_bundle_impl_GetFileNameOfPath(char* path);

#endif