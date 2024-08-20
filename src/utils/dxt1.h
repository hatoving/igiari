#ifndef IGIARI_UTILS_DXT1_H
#define IGIARI_UTILS_DXT1_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void igiari_utils_dxt1_Decode(unsigned long width, unsigned long height, const unsigned char *blockStorage, unsigned char *image);

#endif