#ifndef IGIARI_FMOD_VORBISHEADERS_H
#define IGIARI_FMOD_VORBISHEADERS_H

#include <stdint.h>
#include "../utils/base64.h"

typedef struct {
	uint32_t crc32;
	int blocksize_short;
	int blocksize_long;
	size_t setup_size;
	const char* header;
} igiari_fmod_rebuild_vorbisheader;

#endif