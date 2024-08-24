#ifndef UTILS_UNICODE_H
#define UTILS_UNICODE_H

#include <stdio.h>
#include <stdint.h>

char* igiari_unicode_UTF32_to_8(uint32_t codepoint);
size_t igiari_unicode_UTF8Count(const char *input);
void igiari_unicode_TruncateUTF8String(const char *input, size_t max_chars, char *output);

#endif