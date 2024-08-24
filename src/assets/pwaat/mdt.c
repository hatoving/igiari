#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mdt.h"
#include "../../utils/unicode.h"

int IGIARI_MDTOP_ARG_COUNTS[] = {
    0, 0, 0, 1, 1, 2, 2, 0, 2, 3, 1, 1, 1, 0, 1, 2,
    1, 0, 3, 1, 0, 0, 0, 1, 1, 2, 4, 1, 1, 1, 3, 0,
    1, 0, 2, 2, 0, 1, 1, 2, 1, 1, 3, 0, 1, 0, 0, 2,
    1, 2, 2, 5, 1, 2, 1, 2, 1, 1, 3, 2, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 0, 1, 2, 2, 0, 1, 1, 0, 2, 1, 7,
    1, 2, 1, 0, 2, 1, 2, 1, 0, 1, 1, 2, 3, 0, 0, 3,
    4, 3, 0, 0, 1, 2, 3, 0, 0, 4, 1, 3, 0, 1, 1, 1,
    3, 3, 0, 0, 2, 4, 2, 2, 1, 0, 1, 2, 0, 1, 1, 1
};

/*

1 = BREAK/NEWLINE
11 = SPEED (SPEED)
12 = WAIT (FRAMES [X / 60]) - The original game always assumes that we're running it on 60 FPS, so = (float)FRAMES_TO_WAIT / 60.0f.
46 = HIDE_TEXTBOX
*/

void igiari_mdt_impl_GetMsgCount(igiari_mdt* mdt, FILE* mdt_file) {
    fread(&(mdt->message_count), 2, 1, mdt_file);
    printf("[igiari, mdt] There are %i messages in this MDT file.\n", (mdt->message_count));
    long prev_pos = ftell(mdt_file);
    fseek(mdt_file, 2, SEEK_CUR);

    for (int i = 0; i < mdt->message_count; i++)
    {
        uint32_t offset; fread(&offset, 4, 1, mdt_file);
        if (offset > mdt->file_length) {
            printf("[igiari, mdt] Found illegal offset. (%i > %i)\n", offset, mdt->file_length);

            mdt->message_count--;
            mdt->invalid_offsets++;
        }
    }

    fseek(mdt_file, prev_pos, SEEK_SET);
}

igiari_mdt igiari_mdt_Read(char* path) {
    igiari_mdt mdt;
    FILE* mdt_file = fopen(path, "rb");

    fseek(mdt_file, 0, SEEK_END);
    mdt.file_length = ftell(mdt_file);
    fseek(mdt_file, 0, SEEK_SET);

    igiari_mdt_impl_GetMsgCount(&mdt, mdt_file);
    fseek(mdt_file, 2, SEEK_CUR);

    mdt.message_offsets = malloc(sizeof(uint16_t) * mdt.message_count);
    for (uint16_t i = 0; i < mdt.message_count; i++)
    {
        fread(&mdt.message_offsets[i], 4, 1, mdt_file);
    }
    
    for (uint16_t i = 0; i < mdt.invalid_offsets; i++)
    {
        fseek(mdt_file, 4, SEEK_CUR);
    }
    
    mdt.message_lengths = malloc(sizeof(uint16_t) * mdt.message_count);
    for (uint16_t i = 0; i < mdt.message_count - 1; i++)
    {
        //printf("[igiari, mdt] Message length: %i\n", (mdt.message_offsets[i + 1] - mdt.message_offsets[i]) / 2);
        mdt.message_lengths[i] = (mdt.message_offsets[i + 1] - mdt.message_offsets[i]) / 2;
    }
    mdt.message_lengths[mdt.message_count - 1] = (mdt.file_length - mdt.message_offsets[mdt.message_count - 1]) / 2;

    int break_ = 0;

    int op_size = 0;    
    mdt.operations = calloc(0, sizeof(igiari_mdt_op));

    for (uint16_t i = 0; i < mdt.message_count; i++)
    {
        int op_times = 0;

        //mdt.operations[i] = realloc(mdt.operations[i], (i + 1) * sizeof(igiari_mdt_op));
        /*mdt.operations[i].args = malloc((mdt.message_lengths[i] - op_times) * sizeof(char));
        mdt.operations[i].args[0] = '\0';*/

        for (uint32_t j = 0; j < mdt.message_lengths[i] - op_times; j++)
        {
            if (mdt.file_length - ftell(mdt_file) < sizeof(uint16_t)) {
                // Can't read anymore, break
                break_ = 1;
                break;
            }

            uint16_t op_or_char;
            fread(&op_or_char, sizeof(uint16_t), 1, mdt_file);

            if (op_or_char >= 128) {
                op_or_char -= 128;
                char* utf8_char = igiari_unicode_UTF32_to_8(op_or_char);
                //snprintf(buffer, sizeof(buffer), "\\u%02X", op_or_char);
                mdt.operations = realloc(mdt.operations, (op_size + 1) * sizeof(igiari_mdt_op));

                mdt.operations[op_size].character = utf8_char;
                mdt.operations[op_size].op = 0xFF;

                //printf("[igiari, mtd] OP: 0x%X, STRING: \"%s\", MSG: %i\n", mdt.operations[op_size].op, mdt.operations[op_size].args, op_size);

                op_size += 1;
            } else {
                mdt.operations = realloc(mdt.operations, (op_size + 1) * sizeof(igiari_mdt_op));

                fread(&mdt.operations[op_size].args, 2, IGIARI_MDTOP_ARG_COUNTS[op_or_char], mdt_file);
                mdt.operations[op_size].op = op_or_char;

                op_size += 1;
                op_times += IGIARI_MDTOP_ARG_COUNTS[op_or_char];

                //printf("[igiari, mtd] OP: 0x%X, ARG COUNT: %i\n", mdt.operations[op_size - 1].op, IGIARI_MDTOP_ARG_COUNTS[op_or_char]);
            }
        }
        // msg end
        
        mdt.operations = realloc(mdt.operations, (op_size + 1) * sizeof(igiari_mdt_op));
        mdt.operations[op_size].op = 0xFF + 1;
        op_size += 1;
        //op_times += IGIARI_MDTOP_ARG_COUNTS[op_or_char];
                
        if (break_) {
            break;
        }
    }

    fclose(mdt_file);
    return mdt;
}