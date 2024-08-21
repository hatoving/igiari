#include "rebuild.h"
#include "vorbis_headers.h"
#include "../../utils/reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vorbis/codec.h>

ogg_packet igiari_fmod_rebuild_vorbis_BuildInfoPacket(int blocksize_short, int blocksize_long, uint8_t channels, uint32_t freq) {
    oggpack_buffer buf;
    ogg_packet packet;

    oggpack_writeinit(&buf);
    oggpack_write(&buf, 0x01, 8);  // Packet type (0x01 = identification header)

    const char *magic = "vorbis";
    for (int i = 0; i < 6; i++) {
        oggpack_write(&buf, magic[i], 8);
    }

    oggpack_write(&buf, 0x00, 32);  // Vorbis version (must be 0)
    oggpack_write(&buf, channels, 8);  // Number of channels
    oggpack_write(&buf, freq, 32);  // Sample rate

    oggpack_write(&buf, 0, 32);  // Bitrate maximum (unspecified)
    oggpack_write(&buf, 0, 32);  // Bitrate nominal (unspecified)
    oggpack_write(&buf, 0, 32);  // Bitrate minimum (unspecified)
    
    // Compute log2 block sizes
    int log2_short = 31 - __builtin_clz(blocksize_short);
    int log2_long = 31 - __builtin_clz(blocksize_long);

    oggpack_write(&buf, log2_short, 4);
    oggpack_write(&buf, log2_long, 4);

    oggpack_write(&buf, 1, 1);  // Framing flag

    //printf("[igiari, fmod, rebulid] Channels: %u, Frequency: %u\n", channels, freq);
    //printf("Blocksize Short: %d (log2: %d), Blocksize Long: %d (log2: %d)\n", blocksize_short, log2_short, blocksize_long, log2_long);

    packet.bytes = oggpack_bytes(&buf);
    packet.packet = malloc(packet.bytes);
    memcpy(packet.packet, buf.buffer, packet.bytes);

    packet.b_o_s = 1;
    packet.e_o_s = 0;
    packet.granulepos = 0;
    packet.packetno = 0;

    oggpack_writeclear(&buf);
    return packet;
}

ogg_packet igiari_fmod_rebuild_vorbis_BuildCommentPacket(int loop_start, int loop_end) {
    vorbis_comment comment;

    ogg_packet packet;
    vorbis_comment_init(&comment);
    if (loop_start != 0 && loop_end != 0) {
        char loop_start_str[20];
        sprintf(loop_start_str, "%i", loop_start);
        char loop_end_str[20];
        sprintf(loop_end_str, "%i", loop_end);

        // VERY IMPORTANT. SDL_mixer_ext can recognize these tags and automatically play this shit looped without any more work
        vorbis_comment_add_tag(&comment, "LOOPSTART", loop_start_str);
        vorbis_comment_add_tag(&comment, "LOOPEND", loop_end_str);
    }
    //ogg_packet_clear(&packet);
    vorbis_commentheader_out(&comment, &packet);
    vorbis_comment_clear(&comment);
    return packet;
}
void igiari_fmod_rebuild_CopySampleData(char** final_data, int* current_size, char* sample_bytes, int sample_bytes_len, int prev_pos, ogg_stream_state* os, vorbis_info* info) {
    char* ptr = sample_bytes;
    
    int size = *current_size;
    int packetno = prev_pos;

    int granule_pos = 0;
    int prev_block_size = 0;

    uint16_t packet_size = *(uint16_t*)ptr; ptr += 2;
    while (packet_size)
    {
        packetno += 1;
        ogg_packet p;

        p.packet = malloc(packet_size);
        memcpy(p.packet, ptr, packet_size); ptr += packet_size;

        p.bytes = packet_size;
        p.packetno = packetno;
        p.granulepos = -1;

        int block_size = vorbis_packet_blocksize(info, &p);
        //printf("block_size: %i\n", block_size);

        //printf("len: %i, block size: %i, granule_pos: %i", packet_size, block_size, granule_pos);
        //if (block_size > 0) printf("\n");
        //else printf(" <--- (INVALID.)\n");
        if (block_size < 0) printf("Invlid block_size: %i\n", block_size);

        if((ptr + 2) < (sample_bytes + sample_bytes_len)) {
            packet_size = *(uint16_t*)ptr; ptr += 2;
        } else {
            packet_size = 0;
        }
        p.e_o_s = !packet_size ? 1 : 0;

        granule_pos = prev_block_size ? (granule_pos + (block_size + prev_block_size) / 4) : 0;

        p.granulepos = granule_pos;
        prev_block_size = block_size;

        ogg_stream_packetin(os, &p);

        ogg_page page;
        while(ogg_stream_pageout(os, &page)) {
            *final_data = realloc(*final_data, size + page.header_len + page.body_len);
            memcpy(*final_data + size, page.header, page.header_len);
            memcpy(*final_data + size + page.header_len, page.body, page.body_len);
            size += page.header_len + page.body_len;
        }
    }
    
    *current_size = size;
}

igiari_fmod_rebuild_vorbisheader igiari_fmod_rebuild_vorbisheaders[] = {
    #include "vorbis_headers.inc"
};


char* igiari_fmod_rebuild_vorbis_Convert(igiari_fmod_fsb_sample* sample, int* size, int* loop_start, int* loop_end) {
    igiari_fmod_chunk_vorbis* vorbis_chunk = NULL;
    for (int i = 0; i < sample->metadata->chunk_count; i++)
    {
        if (sample->metadata->chunks[i]->chunk_type == 0x16) {
            vorbis_chunk = sample->metadata->chunks[i]->chunk.vorbis;
        }
    }
    
    if (vorbis_chunk == NULL) {
        printf("[igiari, fmod, rebuild] Sample provided is not OggVorbis.\n");
        return NULL;
    }

    igiari_fmod_rebuild_vorbisheader* header = (igiari_fmod_rebuild_vorbisheader*)malloc(sizeof(igiari_fmod_rebuild_vorbisheader));
    int num_elements = sizeof(igiari_fmod_rebuild_vorbisheaders) / sizeof(igiari_fmod_rebuild_vorbisheaders[0]);
    for (int i = 0; i < num_elements; i++)
    {
        if (igiari_fmod_rebuild_vorbisheaders[i].crc32 == vorbis_chunk->crc32) {
            header = &igiari_fmod_rebuild_vorbisheaders[i];
            //printf("[igiari, fmod, rebulid] Found header! (%i == %i)\n", igiari_fmod_rebuild_vorbisheaders[i].crc32, vorbis_chunk->crc32);
        }
    }
    
    ogg_packet info_packet = igiari_fmod_rebuild_vorbis_BuildInfoPacket(0x100, 0x800, (uint8_t)sample->metadata->num_channels, igiari_fmod_fsb_GetFreq(sample->metadata->freq_id));

    for (int i = 0; i < sample->metadata->chunk_count; i++)
    {
        if (sample->metadata->chunks[i]->chunk_type == 6) {
            *loop_start = sample->metadata->chunks[i]->chunk.loop->loop_start;
            *loop_end = sample->metadata->chunks[i]->chunk.loop->loop_end;
        }
    }

    ogg_packet comment_packet = igiari_fmod_rebuild_vorbis_BuildCommentPacket(*loop_start, *loop_end);
    ogg_packet header_packet;

    header_packet.packet = malloc(header->setup_size);
    memcpy(header_packet.packet, header->header, header->setup_size);

    header_packet.bytes = header->setup_size;

    header_packet.b_o_s = 0;
    header_packet.e_o_s = 0;
    header_packet.granulepos = 0;
    header_packet.packetno = 2;
    
    vorbis_info info;
    vorbis_info_init(&info);
    vorbis_comment comment;
    vorbis_comment_init(&comment);

    ogg_stream_state os;
    ogg_stream_init(&os, 1);

    int synthesis = vorbis_synthesis_headerin(&info, &comment, &info_packet);
    //printf("[igiari, fmod, rebuild] info_packet result: %i\n", synthesis);
    synthesis = vorbis_synthesis_headerin(&info, &comment, &comment_packet);
    //printf("[igiari, fmod, rebuild] comment_packet result: %i\n", synthesis);
    synthesis = vorbis_synthesis_headerin(&info, &comment, &header_packet);
    //printf("[igiari, fmod, rebuild] header_packet result: %i\n", synthesis);

    char* final_data = malloc(1);
    int final_data_size = 0;

    ogg_page page;
    ogg_stream_packetin(&os, &info_packet);
    while(ogg_stream_pageout(&os, &page)) {
        unsigned char *new_data = realloc(final_data, final_data_size + page.header_len + page.body_len);
        final_data = new_data;
        
        memcpy(final_data + final_data_size, page.header, page.header_len);
        memcpy(final_data + final_data_size + page.header_len, page.body, page.body_len);
        final_data_size += page.header_len + page.body_len;
    }
    ogg_page pagec;
    ogg_stream_packetin(&os, &comment_packet);
    while(ogg_stream_pageout(&os, &pagec)) {
        unsigned char *new_data = realloc(final_data, final_data_size + pagec.header_len + pagec.body_len);
        final_data = new_data;
        
        memcpy(final_data + final_data_size, pagec.header, pagec.header_len);
        memcpy(final_data + final_data_size + pagec.header_len, pagec.body, pagec.body_len);
        final_data_size += pagec.header_len + pagec.body_len;
    }
    ogg_page pageh;
    ogg_stream_packetin(&os, &header_packet);
    while(ogg_stream_pageout(&os, &pageh)) {
        unsigned char *new_data = realloc(final_data, final_data_size + pageh.header_len + pageh.body_len);
        final_data = new_data;
        
        memcpy(final_data + final_data_size, pageh.header, pageh.header_len);
        memcpy(final_data + final_data_size + pageh.header_len, pageh.body, pageh.body_len);
        final_data_size += pageh.header_len + pageh.body_len;
    }
    ogg_page final;
    while(ogg_stream_flush(&os, &final)) {
        unsigned char *new_data = realloc(final_data, final_data_size + final.header_len + final.body_len);
        final_data = new_data;
        
        memcpy(final_data + final_data_size, final.header, final.header_len);
        memcpy(final_data + final_data_size + final.header_len, final.body, final.body_len);
        final_data_size += final.header_len + final.body_len;
    }

    igiari_fmod_rebuild_CopySampleData(&final_data, &final_data_size, sample->sample_bytes, sample->sample_len, header_packet.packetno, &os, &info);
    //printf("FINAL size: %i\n", final_data_size);

    *size = final_data_size;
    return final_data;
    //printf("[igiari, fmod, rebuild] block_size result: %i\n", info.bitrate_window);
}