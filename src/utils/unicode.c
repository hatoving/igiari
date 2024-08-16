#include "unicode.h"
#include <stdlib.h>

char* igiari_unicode_UTF32_to_8(uint32_t code_point) {
    char* buffer = malloc(5);
    if (!buffer) return NULL;

    if ((code_point >= 0xFF41 && code_point <= 0xFF5A) || ((code_point >= 0xFF21 && code_point <= 0xFF3A)) || (code_point >= 0xFF00 && code_point <= 0xFF60)) { // FULL-WIDTH characters
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