#include "reader.h"

uint16_t igiari_utils_reader_ReadUInt16_BE(FILE *file) {
    uint8_t bytes[2];
    fread(bytes, 1, 2, file);
    return (bytes[0] << 8) | bytes[1];
}

uint32_t igiari_utils_reader_ReadUInt32_BE(FILE *file) {
    uint8_t bytes[4];
    fread(bytes, 1, 4, file);
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

uint64_t igiari_utils_reader_ReadUInt64_BE(FILE *file) {
    uint8_t bytes[8];
    fread(bytes, 1, 8, file);
    return ((uint64_t)bytes[0] << 56) |
           ((uint64_t)bytes[1] << 48) |
           ((uint64_t)bytes[2] << 40) |
           ((uint64_t)bytes[3] << 32) |
           ((uint64_t)bytes[4] << 24) |
           ((uint64_t)bytes[5] << 16) |
           ((uint64_t)bytes[6] << 8)  |
           (uint64_t)bytes[7];
}

uint32_t igiari_utils_reader_ConvertUInt32_LEtoBE(uint32_t le) {
    return ((le & 0x000000FF) << 24) |
           ((le & 0x0000FF00) << 8)  |
           ((le & 0x00FF0000) >> 8)  |
           ((le & 0xFF000000) >> 24);
}

uint16_t igiari_utils_reader_ConvertUInt16_LEtoBE(uint16_t le) {
    return ((le & 0x00FF) << 8) | 
           ((le & 0xFF00) >> 8);
}

uint64_t igiari_utils_reader_ConvertUInt64_LEtoBE(uint64_t le) {
    // Reorder the bytes
    return ((le & 0x00000000000000FFULL) << 56) |
           ((le & 0x000000000000FF00ULL) << 40) |
           ((le & 0x0000000000FF0000ULL) << 24) |
           ((le & 0x00000000FF000000ULL) << 8)  |
           ((le & 0x000000FF00000000ULL) >> 8)  |
           ((le & 0x0000FF0000000000ULL) >> 24) |
           ((le & 0x00FF000000000000ULL) >> 40) |
           ((le & 0xFF00000000000000ULL) >> 56);
}

uint32_t igiari_utils_reader_ReadUInt24FromPtr(char* ptr) {
    uint32_t x = *(uint8_t*)ptr; ptr += 1;
    uint16_t y = *(uint16_t*)ptr; ptr += 2;
    return x + (y << 8);
}

uint64_t igiari_utils_reader_GetValueFromBits(uint64_t value, int lowest, int num_bits) {
    uint64_t mask = (1ULL << num_bits) - 1;
    return (value >> lowest) & mask; 
}

uint32_t igiari_utils_reader_ReadBitsFromPtr(char** data, int* bit_offset, int num_bits) {
    uint32_t result = 0; // Use uint32_t to hold the result
    while (num_bits > 0) {
        // Calculate the bit position in the current byte
        int byte_offset = *bit_offset / 8;   // Byte offset in the data
        int bit_index = *bit_offset % 8;     // Bit index in the byte

        // Read the current byte
        uint8_t currentByte = *(uint8_t*)(*data + byte_offset);

        // Extract the bit from the current byte
        uint8_t currentBit = (currentByte >> (7 - bit_index)) & 0x01;
        result = (result << 1) | currentBit;

        // Move to the next bit
        (*bit_offset)++;
        num_bits--;

        // If we have moved past the end of the current byte, move to the next byte
        if (*bit_offset % 8 == 0) {
            (*data)++; // Move to the next byte in the data
        }
    }

    return result;
}

char* igiari_utils_reader_ReadStringTilNull_FromPointer(const char* start_ptr, size_t* bytes_read) {
    if (start_ptr == NULL) {
        fprintf(stderr, "Error: Pointer is NULL.\n");
        return NULL;
    }

    size_t buffer_size = 1024;
    char* buffer = (char*)malloc(buffer_size);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return NULL;
    }

    char* ptr = buffer;
    const char* current = start_ptr;

    while (*current != '\0') {
        if (ptr - buffer >= buffer_size - 1) {
            // Buffer is full, resize it
            size_t new_size = buffer_size * 2;
            char* new_buffer = (char*)realloc(buffer, new_size);
            if (new_buffer == NULL) {
                fprintf(stderr, "Error: Memory reallocation failed.\n");
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
            ptr = buffer + (ptr - buffer);
            buffer_size = new_size;
        }
        *ptr++ = *current++;
    }

    // Null-terminate the string
    *ptr = '\0';

    // If *bytes_read is not NULL, set the number of bytes read
    if (bytes_read != NULL) {
        *bytes_read = (size_t)(current - start_ptr) + 1; // +1 for the null terminator
    }

    return buffer;
}

char* igiari_utils_reader_ReadStringTilNull(FILE* file) {
    if (file == NULL) {
        fprintf(stderr, "Error: File pointer is NULL.\n");
        return NULL;
    }

    char* buffer = (char*)malloc(1024);

    char* ptr = buffer;
    int c;

    while ((c = fgetc(file)) != EOF) {
        if (c == '\0') {
            break;
        }
        if (ptr - buffer >= 1024 - 1) {
            fprintf(stderr, "Error: Buffer overflow.\n");
            free(buffer);
            return NULL;
        }
        *ptr++ = (char)c;
    }
    
    *ptr = '\0';

    if (c == EOF && ptr == buffer) {
        free(buffer);
        return NULL;
    }

    return buffer;
}