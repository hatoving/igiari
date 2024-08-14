#ifndef IGIARI_UTILS_READER_H
#define IGIARI_UTILS_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint16_t igiari_utils_reader_ReadUInt16_BE(FILE *file);
uint32_t igiari_utils_reader_ReadUInt32_BE(FILE *file);
uint64_t igiari_utils_reader_ReadUInt64_BE(FILE *file);

uint16_t igiari_utils_reader_ConvertUInt16_LEtoBE(uint16_t le);
uint32_t igiari_utils_reader_ConvertUInt32_LEtoBE(uint32_t le);
uint64_t igiari_utils_reader_ConvertUInt64_LEtoBE(uint64_t le);

char* igiari_utils_reader_ReadStringTilNull(FILE* file);
char* igiari_utils_reader_ReadStringTilNull_FromPointer(const char* start_ptr, size_t* bytes_read);

#endif