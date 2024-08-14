#ifndef boshi_CORE_H
#define boshi_CORE_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <raylib.h>

extern int boshi_GAME_WIDTH;
extern int boshi_GAME_HEIGHT;

extern bool boshi_INITIALIZED;

/* The main game texture. boshi uses this to render things to the screen. */
extern RenderTexture2D boshi_GAME_TEXTURE;

typedef enum {
    /* Pixel-perfect/integer-scaling mode. Scales the game screen to fit via integer scaling. */
    boshi_RENDER_MODE_PIXEL,
    /* Stretch mode. Stretches the entire game screen whatever the current display size is. */
    boshi_RENDER_MODE_STRETCH,
    /* Fit mode. Makes the game screen's aspect ratio fit to the display's aspect ratio.  */
    boshi_RENDER_MODE_FIT
} boshi_core_RenderingMode;
extern boshi_core_RenderingMode boshi_RENDERING_MODE;

typedef enum {
    boshi_ORIGIN_TOPLEFT,
    boshi_ORIGIN_TOPRIGHT,
    boshi_ORIGIN_TOPCENTER,
    boshi_ORIGIN_CENTERLEFT,
    boshi_ORIGIN_CENTERRIGHT,
    boshi_ORIGIN_CENTER,
    boshi_ORIGIN_BOTTOMLEFT,
    boshi_ORIGIN_BOTTOMRIGHT,
    boshi_ORIGIN_BOTTOMCENTER,
    boshi_ORIGIN_CUSTOM
} boshi_core_Origin;
Vector2 boshi_core_GetOrigin(boshi_core_Origin origin);

int boshi_core_Initialize(char* title, int width, int height, int game_width, int game_height, int frame_rate, bool vsync);
void boshi_core_Quit();

void boshi_core_SetRenderMode(boshi_core_RenderingMode mode);

void boshi_core_BeginDrawing();
void boshi_core_EndDrawing();

Texture2D boshi_core_LoadMissingTexture();

#endif