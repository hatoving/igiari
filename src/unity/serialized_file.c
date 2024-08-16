#include "serialized_file.h"
#include "../utils/reader.h"

#include <string.h>

char* igiari_unity_serializedfile_impl_ReadStringFromNode(uint8_t* str, uint32_t value) {
    int isOffset = (value & 0x80000000) == 0;

    if (isOffset) {
        uint32_t offset = value;
        const uint8_t* strStart = str + offset;
        size_t len = 0;

        while (strStart[len] != '\0') {
            len++;
        }

        char* result = (char*)malloc(len + 1);
        memcpy(result, strStart, len);
        result[len] = '\0';

        return result;
    } else {
        uint32_t offset = value & 0x7FFFFFFF;
        size_t length = snprintf(NULL, 0, "%u", offset) + 1;
        char* result = (char*)malloc(length);
        snprintf(result, length, "%u", offset);

        return result;
    }
}

void igiari_unity_serizaliedfile_impl_ReadTypeTreeNodesFromData(igiari_unity_serizaliedfile* file, igiari_unity_serizaliedfile_stype* stype) {
    int node_count = (file->header.endianess == 0) ? *(int32_t*)file->ptr : igiari_utils_reader_ConvertUInt16_LEtoBE(*(int32_t*)file->ptr); file->ptr += 4;
    int string_buf_size = (file->header.endianess == 0) ? *(int32_t*)file->ptr : igiari_utils_reader_ConvertUInt16_LEtoBE(*(int32_t*)file->ptr); file->ptr += 4;

    stype->type.nodes = malloc(sizeof(igiari_unity_serizaliedfile_typetreenode));
    int count = 0;
    for (int i = 0; i < node_count; i++)
    {
        igiari_unity_serizaliedfile_typetreenode dummy;

        dummy.version = (file->header.endianess == 0) ? *(uint16_t*)file->ptr : igiari_utils_reader_ConvertUInt16_LEtoBE(*(uint16_t*)file->ptr); file->ptr += 2;
        dummy.level = *(uint8_t*)file->ptr; file->ptr += 1;
        dummy.type_flags = *(uint8_t*)file->ptr; file->ptr += 1;
        dummy.type_str_offset = (file->header.endianess == 0) ? *(uint32_t*)file->ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(uint32_t*)file->ptr); file->ptr += 4;
        dummy.name_str_offset = (file->header.endianess == 0) ? *(uint32_t*)file->ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(uint32_t*)file->ptr); file->ptr += 4;
        dummy.bytesize = (file->header.endianess == 0) ? *(int32_t*)file->ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file->ptr); file->ptr += 4;
        dummy.index = (file->header.endianess == 0) ? *(int32_t*)file->ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file->ptr); file->ptr += 4;
        dummy.meta_flag = (file->header.endianess == 0) ? *(int32_t*)file->ptr : igiari_utils_reader_ConvertUInt16_LEtoBE(*(int32_t*)file->ptr); file->ptr += 4;

        stype->type.nodes = realloc(stype->type.nodes, (count + 1) * sizeof(igiari_unity_serizaliedfile_typetreenode));
        stype->type.nodes[count] = dummy;
        count += 1;
    }

    stype->type.string_buf = malloc(string_buf_size);
    memcpy(stype->type.string_buf, file->ptr, string_buf_size); file->ptr += string_buf_size;

    for (int i = 0; i < count; i++)
    {
        // Calculate lengths of substrings
        stype->type.nodes[i].type = igiari_unity_serializedfile_impl_ReadStringFromNode(stype->type.string_buf, stype->type.nodes[i].type_str_offset);
        stype->type.nodes[i].name = igiari_unity_serializedfile_impl_ReadStringFromNode(stype->type.string_buf, stype->type.nodes[i].name_str_offset);
        //printf("type: \"%s\", name: \"%s\"\n", stype->type.nodes[i].type, stype->type.nodes[i].name);
    }
}

igiari_unity_serizaliedfile_stype igiari_unity_serizaliedfile_impl_ReadSerializedTypeFromData(igiari_unity_serizaliedfile* file, int is_ref_type) {
    igiari_unity_serizaliedfile_stype dummy;

    dummy.class_id = (file->header.endianess == 0) ? *(int32_t*)file->ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file->ptr); file->ptr += 4;
    if (file->header.version >= 16) {
        dummy.is_stripped_type = *(uint8_t*)file->ptr; file->ptr += 1;
    }
    if (file->header.version >= 17) {
        dummy.script_type_index = (file->header.endianess == 0) ? *(int16_t*)file->ptr : igiari_utils_reader_ConvertUInt16_LEtoBE(*(int16_t*)file->ptr); file->ptr += 2;
    }
    if (file->header.version >= 13) {
        //dummy.script_type_index = (file->header.endianess == 0) ? *(uint16_t*)file->ptr : igiari_utils_reader_ConvertUInt16_LEtoBE(*(uint16_t*)file->ptr); file->ptr += 2;
        if (is_ref_type == 1 && dummy.script_type_index >= 0) {
            dummy.script_id = malloc(17);
            memcpy(dummy.script_id, file->ptr, 16); file->ptr += 16;
            //printf("[igiari, unity, stype] script_id: %s\n", dummy.script_id);
        } else if ((file->header.version < 16 && dummy.class_id < 0) || (file->header.version >= 16 && dummy.class_id == 114)) { 
            dummy.script_id = malloc(17);
            memcpy(dummy.script_id, file->ptr, 16); file->ptr += 16;
            //printf("[igiari, unity, stype] script_id: %s\n", dummy.script_id);
        }
        dummy.old_type_hash = malloc(17);
        memcpy(dummy.old_type_hash, file->ptr, 16); file->ptr += 16;
        //printf("[igiari, unity, stype] old_type_hash: %s\n", dummy.old_type_hash);
    }

    if (file->enable_type_tree) {
        if (file->header.version >= 12 || file->header.version == 10) {
            igiari_unity_serizaliedfile_impl_ReadTypeTreeNodesFromData(file, &dummy);
        }
    }

    return dummy;
}

igiari_unity_serizaliedfile igiari_unity_serizaliedfile_ReadFromData(unsigned char* data, long offset) {
    igiari_unity_serizaliedfile file;

    file.ptr = data + offset;
    //printf("[igiari, unity, sfile] hex: %X\n", file.ptr);

    file.header.metadata_size = igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;
    file.header.file_size = igiari_utils_reader_ConvertUInt32_LEtoBE(*(uint32_t*)file.ptr); file.ptr += 4;
    file.header.version = igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;
    file.header.data_offset = igiari_utils_reader_ConvertUInt32_LEtoBE(*(uint32_t*)file.ptr); file.ptr += 4;

    if (file.header.version >= 9) {
        file.header.endianess = *(uint8_t*)file.ptr; file.ptr += 1;
        file.ptr += 3;
    } else {
        const char* new_ptr = data + (offset + (file.header.file_size - file.header.metadata_size));
        file.ptr = new_ptr;
        file.header.endianess = *(uint8_t*)file.ptr; file.ptr += 1;
    }

    if (file.header.version >= 7) {
        size_t bytes_read = 0;
        file.unity_version = igiari_utils_reader_ReadStringTilNull_FromPointer(file.ptr, &bytes_read); file.ptr += bytes_read;
    }
    if (file.header.version >= 8) {
        file.target_platform = (file.header.endianess == 0) ? *(int32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;
    }
    if (file.header.version >= 13) {
        file.enable_type_tree = *(uint8_t*)file.ptr; file.ptr += 1;
    } else {
        file.enable_type_tree = 1;
    }

    int32_t type_count = (file.header.endianess == 0) ? *(int32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;
    //printf("[igiari, unity, sfile] type_count: %i\n", type_count);
    file.types = malloc(sizeof(igiari_unity_serizaliedfile_stype));
    int count = 0;
    for (int i = 0; i < type_count; i++)
    {
        igiari_unity_serizaliedfile_stype dummy = igiari_unity_serizaliedfile_impl_ReadSerializedTypeFromData(&file, 0);
        file.types = realloc(file.types, (count + 1) * sizeof(igiari_unity_serizaliedfile_stype));
        file.types[count] = dummy;
        count++;
    }

    file.big_id_enabled = 0;

    int32_t object_count = (file.header.endianess == 0) ? *(int32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;
    //printf("[igiari, unity, sfile] object_count: %i, hex: %X\n", object_count, file.ptr);
    file.object_infos = malloc(sizeof(igiari_unity_objectinfo));
    int object_arr_count = 0;
    for (int i = 0; i < object_count; i++)
    {
        igiari_unity_objectinfo dummy;

        if (file.big_id_enabled != 0) {
            dummy.path_id = (file.header.endianess == 0) ? *(int64_t*)file.ptr : igiari_utils_reader_ConvertUInt64_LEtoBE(*(int64_t*)file.ptr); file.ptr += 8;
        } else if (file.header.version < 14) {
            dummy.path_id = (file.header.endianess == 0) ? *(int32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;
        } else {
            uintptr_t addr = (uintptr_t)file.ptr;
            addr = (addr + 3) & ~((uintptr_t)3);
            file.ptr = (const char*)addr;

            dummy.path_id = (file.header.endianess == 0) ? *(int64_t*)file.ptr : igiari_utils_reader_ConvertUInt64_LEtoBE(*(int64_t*)file.ptr); file.ptr += 8;
        }

        dummy.byte_start = (file.header.endianess == 0) ? *(uint32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(uint32_t*)file.ptr); file.ptr += 4;
        dummy.byte_start += file.header.data_offset;
        dummy.byte_size = (file.header.endianess == 0) ? *(uint32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(uint32_t*)file.ptr); file.ptr += 4;
        dummy.type_id = (file.header.endianess == 0) ? *(int32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;

        if (file.header.version < 16) {

        } else {
            dummy.stype = file.types[dummy.type_id];
            dummy.class_id = file.types[dummy.type_id].class_id;
        }
        file.object_infos = realloc(file.object_infos, (object_arr_count + 1) * sizeof(igiari_unity_objectinfo));
        file.object_infos[object_arr_count] = dummy;
        object_arr_count++;
    }
    file.object_info_count = object_arr_count;

    if (file.header.version >= 11) {
        int script_count = (file.header.endianess == 0) ? *(int32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;
        file.script_types = malloc(sizeof(igiari_unity_serializedfile_sobjectidentifier));
        int script_arr_count = 0;
        //printf("[igiari, unity, sfile] script_count: %i\n", script_count);
        for (int i = 0; i < script_count; i++)
        {
            igiari_unity_serializedfile_sobjectidentifier dummy;

            dummy.index = (file.header.endianess == 0) ? *(int32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;

            if (file.header.version < 14) {

            } else {
                uintptr_t addr = (uintptr_t)file.ptr;
                addr = (addr + 3) & ~((uintptr_t)3);
                file.ptr = (const char*)addr;

                dummy.in_file = (file.header.endianess == 0) ? *(int64_t*)file.ptr : igiari_utils_reader_ConvertUInt64_LEtoBE(*(int64_t*)file.ptr); file.ptr += 8;
            }

            file.script_types = realloc(file.object_infos, (script_arr_count + 1) * sizeof(igiari_unity_serializedfile_sobjectidentifier));
            file.script_types[script_arr_count] = dummy;
            script_arr_count++;
        }
    }

    int32_t externals_count = (file.header.endianess == 0) ? *(int32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;

    file.file_identifiers = malloc(sizeof(igiari_unity_serializedfile_fileidentifier));
    int externals_arr_count = 0;

    for (int i = 0; i < externals_count; i++)
    {
        igiari_unity_serializedfile_fileidentifier dummy;
        size_t bytes_read = 0;

        if (file.header.version >= 6) {
            igiari_utils_reader_ReadStringTilNull_FromPointer(file.ptr, &bytes_read); file.ptr += bytes_read;
        }
        if (file.header.version >= 5) {
            dummy.guid = malloc(16);
            memcpy(dummy.guid, file.ptr, 16);

            dummy.type = (file.header.endianess == 0) ? *(int32_t*)file.ptr : igiari_utils_reader_ConvertUInt32_LEtoBE(*(int32_t*)file.ptr); file.ptr += 4;
        }
        dummy.path_name = igiari_utils_reader_ReadStringTilNull_FromPointer(file.ptr, &bytes_read); file.ptr += bytes_read;
        dummy.file_name = igiari_utils_reader_ReadStringTilNull_FromPointer(file.ptr, &bytes_read); file.ptr += bytes_read;

        file.file_identifiers = realloc(file.file_identifiers, (externals_arr_count + 1) * sizeof(igiari_unity_objectinfo));
        file.file_identifiers[externals_arr_count] = dummy;
        externals_arr_count++;
    }

    if (file.header.version >= 5) {
        size_t bytes_read = 0;
        file.user_info = igiari_utils_reader_ReadStringTilNull_FromPointer(file.ptr, &bytes_read); file.ptr += bytes_read;
    }
    return file;
    //printf("[igiari, unity, sfile] externals_count: %s\n", file.user_info);
}

igiari_unity_objectinfo* igiari_unity_serializedfile_GetAllObjectsOfType(igiari_unity_serizaliedfile* file, int type, int* count) {
    igiari_unity_objectinfo* info_array = malloc(sizeof(igiari_unity_objectinfo));
    int info_count = 0;

    for (int i = 0; i < file->object_info_count; i++)
    {
        if (file->object_infos[i].class_id == type) {
            info_array = realloc(info_array, (info_count + 1) * sizeof(igiari_unity_objectinfo));
            info_array[info_count] = file->object_infos[i];
            info_count++;
        }
    }

    *count = info_count;
    return info_array;
}