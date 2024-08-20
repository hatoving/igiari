#include "dxt5.h"

// Pack RGBA components into a single unsigned long (32-bit) value
unsigned long igiari_utils_dxt5_PackRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return ((r << 24) | (g << 16) | (b << 8) | a);
}

// Decompress a single 4x4 DXT5 block into RGBA format
void igiari_utils_dxt5_DecompressBlockDXT5(unsigned long x, unsigned long y, unsigned long width, const unsigned char *blockStorage, unsigned char *image)
{
    unsigned char alpha0 = blockStorage[0];
    unsigned char alpha1 = blockStorage[1];

    const unsigned char *bits = blockStorage + 2;
    unsigned long alphaCode1 = bits[2] | (bits[3] << 8) | (bits[4] << 16) | (bits[5] << 24);
    unsigned short alphaCode2 = bits[0] | (bits[1] << 8);

    unsigned short color0 = *(unsigned short *)(blockStorage + 8);
    unsigned short color1 = *(unsigned short *)(blockStorage + 10);

    unsigned char r0 = (color0 >> 11) * 255 / 31;
    unsigned char g0 = ((color0 & 0x07E0) >> 5) * 255 / 63;
    unsigned char b0 = (color0 & 0x001F) * 255 / 31;

    unsigned char r1 = (color1 >> 11) * 255 / 31;
    unsigned char g1 = ((color1 & 0x07E0) >> 5) * 255 / 63;
    unsigned char b1 = (color1 & 0x001F) * 255 / 31;

    unsigned long code = *(unsigned long *)(blockStorage + 12);

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            int alphaCodeIndex = 3 * (4 * j + i);
            int alphaCode;

            if (alphaCodeIndex <= 12)
            {
                alphaCode = (alphaCode2 >> alphaCodeIndex) & 0x07;
            }
            else if (alphaCodeIndex == 15)
            {
                alphaCode = (alphaCode2 >> 15) | ((alphaCode1 << 1) & 0x06);
            }
            else
            {
                alphaCode = (alphaCode1 >> (alphaCodeIndex - 16)) & 0x07;
            }

            unsigned char finalAlpha;
            if (alphaCode == 0)
            {
                finalAlpha = alpha0;
            }
            else if (alphaCode == 1)
            {
                finalAlpha = alpha1;
            }
            else
            {
                if (alpha0 > alpha1)
                {
                    finalAlpha = ((8 - alphaCode) * alpha0 + (alphaCode - 1) * alpha1) / 7;
                }
                else
                {
                    if (alphaCode == 6)
                        finalAlpha = 0;
                    else if (alphaCode == 7)
                        finalAlpha = 255;
                    else
                        finalAlpha = ((6 - alphaCode) * alpha0 + (alphaCode - 1) * alpha1) / 5;
                }
            }

            unsigned char colorCode = (code >> (2 * (4 * j + i))) & 0x03;

            unsigned char r, g, b;

            switch (colorCode)
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

            if (x + i < width && y + j < width)
            {
                unsigned long pixelIndex = ((y + j) * width + (x + i)) * 4;
                image[pixelIndex] = r;       // R
                image[pixelIndex + 1] = g;   // G
                image[pixelIndex + 2] = b;   // B
                image[pixelIndex + 3] = finalAlpha; // A
            }
        }
    }
}

// Decompress the entire texture
void igiari_utils_dxt5_Decode(unsigned long width, unsigned long height, const unsigned char *blockStorage, unsigned char *image)
{
    unsigned long blockCountX = (width + 3) / 4;
    unsigned long blockCountY = (height + 3) / 4;

    for (unsigned long j = 0; j < blockCountY; j++)
    {
        for (unsigned long i = 0; i < blockCountX; i++)
        {
            unsigned long blockOffset = (j * blockCountX + i) * 16;
            igiari_utils_dxt5_DecompressBlockDXT5(i * 4, j * 4, width, blockStorage + blockOffset, image);
        }
    }
}
