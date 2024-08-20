#include "dxt1.h"

unsigned long igiari_utils_dxt1_PackRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return ((r << 24) | (g << 16) | (b << 8) | a);
}

void igiari_utils_dxt1_DecompressBlockDXT1(unsigned long x, unsigned long y, unsigned long width, const unsigned char *blockStorage, unsigned char *image)
{
    unsigned short color0 = *(unsigned short*)blockStorage;
    unsigned short color1 = *(unsigned short*)(blockStorage + 2);

    unsigned char r0 = (color0 >> 11) * 255 / 31;
    unsigned char g0 = ((color0 >> 5) & 0x3F) * 255 / 63;
    unsigned char b0 = (color0 & 0x1F) * 255 / 31;

    unsigned char r1 = (color1 >> 11) * 255 / 31;
    unsigned char g1 = ((color1 >> 5) & 0x3F) * 255 / 63;
    unsigned char b1 = (color1 & 0x1F) * 255 / 31;

    unsigned short code = *(unsigned short*)(blockStorage + 4);

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            unsigned char positionCode = (code >> (2 * (4 * j + i))) & 0x03;
            unsigned char r, g, b;

            if (color0 > color1)
            {
                switch (positionCode)
                {
                    case 0:
                        r = r0;
                        g = g0;
                        b = b0;
                        break;
                    case 1:
                        r = r1;
                        g = g1;
                        b = b1;
                        break;
                    case 2:
                        r = (2 * r0 + r1) / 3;
                        g = (2 * g0 + g1) / 3;
                        b = (2 * b0 + b1) / 3;
                        break;
                    case 3:
                        r = (r0 + 2 * r1) / 3;
                        g = (g0 + 2 * g1) / 3;
                        b = (b0 + 2 * b1) / 3;
                        break;
                }
            }
            else
            {
                switch (positionCode)
                {
                    case 0:
                        r = r0;
                        g = g0;
                        b = b0;
                        break;
                    case 1:
                        r = r1;
                        g = g1;
                        b = b1;
                        break;
                    case 2:
                        r = (r0 + r1) / 2;
                        g = (g0 + g1) / 2;
                        b = (b0 + b1) / 2;
                        break;
                    case 3:
                        r = 0;
                        g = 0;
                        b = 0;
                        break;
                }
            }

            if (x + i < width && y + j < width)
            {
                unsigned long pixelIndex = ((y + j) * width + (x + i)) * 4;
                image[pixelIndex] = r;       // R
                image[pixelIndex + 1] = g;   // G
                image[pixelIndex + 2] = b;   // B
                image[pixelIndex + 3] = 255; // A
            }
        }
    }
}

// Decompress the entire texture
void igiari_utils_dxt1_Decode(unsigned long width, unsigned long height, const unsigned char *blockStorage, unsigned char *image)
{
    unsigned long blockCountX = (width + 3) / 4;
    unsigned long blockCountY = (height + 3) / 4;

    for (unsigned long j = 0; j < blockCountY; j++)
    {
        for (unsigned long i = 0; i < blockCountX; i++)
        {
            unsigned long blockOffset = (j * blockCountX + i) * 8;
            igiari_utils_dxt1_DecompressBlockDXT1(i * 4, j * 4, width, blockStorage + blockOffset, image);
        }
    }
}
