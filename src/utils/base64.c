#include "base64.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const char igiari_utils_base64_table[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

unsigned char* igiari_utils_base64_Decode(char* base64, int* length) {
    unsigned char* data;

    unsigned char base64_decode_table[256];
    for (int i = 0; i < 64; ++i) {
        base64_decode_table[(unsigned char)igiari_utils_base64_table[i]] = i;
    }

    size_t len = strlen(base64);
    if (len % 4 != 0) return NULL; // Invalid Base64 string length

    size_t out_len = len / 4 * 3;
    if (base64[len - 1] == '=') out_len--;
    if (base64[len - 2] == '=') out_len--;

    data = (unsigned char *)malloc(out_len);
    if (*data == NULL) return NULL; // Memory allocation failed

    size_t i, j;
    for (i = 0, j = 0; i < len;) {
        uint32_t octet_a = base64_decode_table[(unsigned char)base64[i++]];
        uint32_t octet_b = base64_decode_table[(unsigned char)base64[i++]];
        uint32_t octet_c = base64_decode_table[(unsigned char)base64[i++]];
        uint32_t octet_d = base64_decode_table[(unsigned char)base64[i++]];

        uint32_t triple = (octet_a << 18) | (octet_b << 12) | (octet_c << 6) | octet_d;

        if (j < out_len) (data)[j++] = (triple >> 16) & 0xFF;
        if (j < out_len) (data)[j++] = (triple >> 8) & 0xFF;
        if (j < out_len) (data)[j++] = triple & 0xFF;
    }

    *length = out_len;
    return data;
}