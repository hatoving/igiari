#ifndef IGIARI_UNITY_SERIALZED_FILE_H
#define IGIARI_UNITY_SERIALZED_FILE_H

#include "object.h"
#include <stdint.h>

typedef struct {
    char* type;
    char* name;
    uint32_t bytesize;
    uint32_t index;
    uint32_t type_flags;
    uint32_t version;
    uint32_t meta_flag;
    uint32_t level;
    uint32_t type_str_offset;
    uint32_t name_str_offset;
    uint16_t ref_type_hash;
} igiari_unity_serizaliedfile_typetreenode;

typedef struct {
    igiari_unity_serizaliedfile_typetreenode* nodes;
    uint8_t* string_buf;
} igiari_unity_serizaliedfile_typetree;

typedef struct {
    uint32_t class_id;
    uint8_t is_stripped_type;
    uint16_t script_type_index;
    igiari_unity_serizaliedfile_typetree type;
    char* script_id;
    char* old_type_hash;
    int* type_dependencies;
    char* class_name;
    char* name_space;
    char* asm_name;
} igiari_unity_serizaliedfile_stype;

typedef struct {
    long byte_start;
    uint32_t byte_size;
    uint32_t type_id;
    uint32_t class_id;
    uint16_t is_destroyed;
    uint8_t stripped;

    long path_id;
    igiari_unity_serizaliedfile_stype stype;
} igiari_unity_objectinfo;

typedef struct {
    uint32_t metadata_size;
    uint32_t file_size;
    uint32_t version;
    uint32_t data_offset;
    uint8_t endianess;
    char* reserved;
} igiari_unity_serializedfile_header;

typedef struct {
    uint32_t index;
    long in_file;
} igiari_unity_serializedfile_sobjectidentifier;

typedef struct {
    uint8_t* guid;
    int type;
    char* path_name;
    char* file_name;
} igiari_unity_serializedfile_fileidentifier;

typedef struct {
    igiari_unity_serializedfile_header header;
    
    int* version;
    char* ptr;

    char build_type;
    int target_platform;

    char* unity_version;
    char* user_info;

    int object_info_count;
    igiari_unity_objectinfo* object_infos;

    int enable_type_tree;
    igiari_unity_serizaliedfile_stype* types;

    igiari_unity_serializedfile_sobjectidentifier* script_types;
    igiari_unity_serializedfile_fileidentifier* file_identifiers;

    int big_id_enabled;
} igiari_unity_serizaliedfile;
igiari_unity_serizaliedfile igiari_unity_serizaliedfile_ReadFromData(unsigned char* data, long offset);

igiari_unity_objectinfo* igiari_unity_serializedfile_GetAllObjectsOfType(igiari_unity_serizaliedfile* file, int type, int* count);

#endif