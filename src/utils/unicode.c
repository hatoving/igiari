#include "unicode.h"

#include <stdbool.h>
#include <stdlib.h>

char* igiari_unicode_UTF32_to_8(uint32_t code_point) {
    char* buffer = malloc(5);
    if (!buffer) return NULL;

    if (code_point == 0xFF0D) { //FULL-WIDTH hiphen
        buffer[0] = '-';
        buffer[1] = '\0';
    }  else if ((code_point >= 0xFF41 && code_point <= 0xFF5A) || ((code_point >= 0xFF21 && code_point <= 0xFF3A)) || (code_point >= 0xFF00 && code_point <= 0xFF60)) { // FULL-WIDTH characters
        buffer[0] = (char) code_point - 0xFEE0;
        buffer[1] = '\0';
    } else if (code_point == 0x3000) { //FULL-WIDTH space
        buffer[0] = ' ';
        buffer[1] = '\0';
    } else if (code_point <= 0x7F) {
        buffer[0] = (char) code_point;
        buffer[1] = '\0';
    } else if (code_point <= 0x7FF) {
        buffer[0] = (char) (0xC0 | (code_point >> 6));
        buffer[1] = (char) (0x80 | (code_point & 0x3F));
        buffer[2] = '\0';
    } else if (code_point <= 0xFFFF) {
        buffer[0] = (char) (0xE0 | (code_point >> 12));
        buffer[1] = (char) (0x80 | ((code_point >> 6) & 0x3F));
        buffer[2] = (char) (0x80 | (code_point & 0x3F));
        buffer[3] = '\0';
    } else if (code_point <= 0x10FFFF) {
        buffer[0] = (char) (0xF0 | (code_point >> 18));
        buffer[1] = (char) (0x80 | ((code_point >> 12) & 0x3F));
        buffer[2] = (char) (0x80 | ((code_point >> 6) & 0x3F));
        buffer[3] = (char) (0x80 | (code_point & 0x3F));
        buffer[4] = '\0';
    } else {
        free(buffer);
        return NULL; // Invalid code point
    }

    return buffer;
}

bool igiari_unicode_IsContinuationByte(unsigned char byte) {
    return (byte & 0x80) != 0 && (byte & 0x40) == 0;
}

// Function to count UTF-8 characters in a string
size_t igiari_unicode_UTF8Count(const char *input) {
    size_t count = 0;
    size_t i = 0;

    while (input[i] != '\0') {
        unsigned char byte = (unsigned char)input[i];
        if ((byte & 0x80) == 0) {
            // 1-byte character (ASCII)
            count++;
            i++;
        } else if ((byte & 0xE0) == 0xC0) {
            // 2-byte character
            count++;
            i += 2;
        } else if ((byte & 0xF0) == 0xE0) {
            // 3-byte character
            count++;
            i += 3;
        } else if ((byte & 0xF8) == 0xF0) {
            // 4-byte character (not commonly used)
            count++;
            i += 4;
        }
    }

    return count;
}

void igiari_unicode_TruncateUTF8String(const char *input, size_t max_chars, char *output) {
    size_t char_count = 0;
    size_t byte_index = 0;

    while (input[byte_index] != '\0' && char_count < max_chars) {
        unsigned char byte = (unsigned char)input[byte_index];
        if ((byte & 0x80) == 0) {
            // 1-byte character (ASCII)
            output[byte_index] = input[byte_index];
            byte_index++;
        } else if ((byte & 0xE0) == 0xC0) {
            // 2-byte character
            if (input[byte_index + 1] != '\0' && igiari_unicode_IsContinuationByte(input[byte_index + 1])) {
                output[byte_index] = input[byte_index];
                output[byte_index + 1] = input[byte_index + 1];
                byte_index += 2;
            } else {
                break; // Malformed UTF-8 sequence
            }
        } else if ((byte & 0xF0) == 0xE0) {
            // 3-byte character
            if (input[byte_index + 1] != '\0' && input[byte_index + 2] != '\0' &&
                igiari_unicode_IsContinuationByte(input[byte_index + 1]) && igiari_unicode_IsContinuationByte(input[byte_index + 2])) {
                output[byte_index] = input[byte_index];
                output[byte_index + 1] = input[byte_index + 1];
                output[byte_index + 2] = input[byte_index + 2];
                byte_index += 3;
            } else {
                break; // Malformed UTF-8 sequence
            }
        } else if ((byte & 0xF8) == 0xF0) {
            // 4-byte character (not commonly used)
            if (input[byte_index + 1] != '\0' && input[byte_index + 2] != '\0' && input[byte_index + 3] != '\0' &&
                igiari_unicode_IsContinuationByte(input[byte_index + 1]) && igiari_unicode_IsContinuationByte(input[byte_index + 2]) && igiari_unicode_IsContinuationByte(input[byte_index + 3])) {
                output[byte_index] = input[byte_index];
                output[byte_index + 1] = input[byte_index + 1];
                output[byte_index + 2] = input[byte_index + 2];
                output[byte_index + 3] = input[byte_index + 3];
                byte_index += 4;
            } else {
                break; // Malformed UTF-8 sequence
            }
        }

        char_count++;
    }

    // Null-terminate the truncated string
    output[byte_index] = '\0';
}