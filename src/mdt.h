#ifndef igiari_MDT_H
#define igiari_MDT_H

#include <stdint.h>

typedef struct {
    uint16_t op;
    uint16_t* args;
    char* text;
} igiari_mdt_op;

typedef struct {
    long file_length;
    uint16_t message_count;
    uint16_t invalid_offsets;

    uint16_t* message_offsets;
    uint16_t* message_lengths;

    igiari_mdt_op* operations;
} igiari_mdt;
igiari_mdt igiari_mdt_Read(char* path);

#endif