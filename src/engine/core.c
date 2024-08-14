#include "core.h"
#include "missing_texture.h"

int boshi_GAME_WIDTH = -1;
int boshi_GAME_HEIGHT = -1;

bool boshi_INITIALIZED = false;
RenderTexture2D boshi_GAME_TEXTURE;

boshi_core_RenderingMode boshi_RENDERING_MODE = boshi_RENDER_MODE_PIXEL;

int boshi_core_Initialize(char* title, int width, int height, int game_width, int game_height, int frame_rate, bool vsync) {
    printf("[boshi] Initing... (Game Size: {%i, %i} / FPS: %i / V-Sync: %i)\n", game_width, game_height, frame_rate, vsync);

    boshi_GAME_WIDTH = game_width;
    boshi_GAME_HEIGHT = game_height;
    
    InitWindow(width, height, title);
    SetTargetFPS(frame_rate);
    SetWindowMinSize(boshi_GAME_WIDTH, boshi_GAME_HEIGHT);
    
    boshi_GAME_TEXTURE = LoadRenderTexture(boshi_GAME_WIDTH, boshi_GAME_HEIGHT);
    //SetTextureFilter(boshi_GAME_TEXTURE.texture, TEXTURE_FILTER_BILINEAR);

    boshi_INITIALIZED = true;
    //SetTextureFilter(boshi_GAME_TEXTURE.texture, TEXTURE_FILTER_LINEAR);

    return 0;
}

void boshi_core_BeginDrawing() {
    BeginTextureMode(boshi_GAME_TEXTURE);
}

void boshi_core_SetRenderMode(boshi_core_RenderingMode mode) {
    boshi_RENDERING_MODE = mode;
}

void boshi_core_impl_HandleRenderMode() {
    switch (boshi_RENDERING_MODE) {
        case boshi_RENDER_MODE_PIXEL: {
            int scale_x = GetScreenWidth() / boshi_GAME_WIDTH;
            int scale_y = GetScreenHeight() / boshi_GAME_HEIGHT;

            // Choose the smaller scale factor to maintain aspect ratio
            int scale = (scale_x < scale_y) ? scale_x : scale_y;
            
            DrawTexturePro(
                boshi_GAME_TEXTURE.texture,
                (Rectangle){0, 0, boshi_GAME_TEXTURE.texture.width, -boshi_GAME_TEXTURE.texture.height, },
                (Rectangle){(GetScreenWidth() - (boshi_GAME_WIDTH * scale)) / 2,
                            (GetScreenHeight() - (boshi_GAME_HEIGHT * scale)) / 2,
                            boshi_GAME_TEXTURE.texture.width * scale,
                            boshi_GAME_TEXTURE.texture.height * scale, },
                (Vector2){ 0, 0 },
                0,
                WHITE
            );
            break;
        }
        case boshi_RENDER_MODE_FIT: {
            float texture_aspect_ratio = (float)boshi_GAME_WIDTH / (float)boshi_GAME_HEIGHT;
            float window_aspect_ratio = (float)GetScreenWidth() / (float)GetScreenHeight();

            // Calculate the scaling factors
            float scale_x, scale_y;
            if (texture_aspect_ratio > window_aspect_ratio) {
                // Texture is wider relative to its height
                scale_x = (float)GetScreenWidth() / (float)boshi_GAME_WIDTH;
                scale_y = scale_x; // Maintain the aspect ratio
            } else {
                // Texture is taller relative to its width
                scale_y = (float)GetScreenHeight() / (float)boshi_GAME_HEIGHT;
                scale_x = scale_y; // Maintain the aspect ratio
            }
            
            //DrawTexture(boshi_GAME_TEXTURE.texture, 0, 0, WHITE);
            DrawTexturePro(
                boshi_GAME_TEXTURE.texture,
                (Rectangle){0, 0, boshi_GAME_TEXTURE.texture.width, -boshi_GAME_TEXTURE.texture.height, },
                (Rectangle){(GetScreenWidth() - (boshi_GAME_WIDTH * scale_x)) / 2,
                            (GetScreenHeight() - (boshi_GAME_HEIGHT * scale_y)) / 2,
                            boshi_GAME_TEXTURE.texture.width * scale_x,
                            boshi_GAME_TEXTURE.texture.height * scale_y, },
                (Vector2){ 0, 0 },
                0,
                WHITE
            );
        };
        case boshi_RENDER_MODE_STRETCH: {
            DrawTexturePro(
                boshi_GAME_TEXTURE.texture,
                (Rectangle){0, 0, boshi_GAME_TEXTURE.texture.width, -boshi_GAME_TEXTURE.texture.height, },
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){ 0, 0 },
                0,
                WHITE
            );
            break;
        }
    }
}

void boshi_core_EndDrawing() {
    EndTextureMode();
    
    BeginDrawing();
        ClearBackground(BLACK);
        boshi_core_impl_HandleRenderMode();
    EndDrawing();
}

Vector2 boshi_core_GetOrigin(boshi_core_Origin origin) {
    switch (origin) {
        case boshi_ORIGIN_TOPLEFT: {
            return (Vector2){0.0, 0.0};
            break;
        }
        case boshi_ORIGIN_TOPRIGHT: {
            return (Vector2){1.0, 0.0};
            break;
        }
        case boshi_ORIGIN_TOPCENTER: {
            return (Vector2){0.5, 0.0};
            break;
        }
        case boshi_ORIGIN_CENTERLEFT: {
            return (Vector2){0.0, 0.5};
            break;
        }
        case boshi_ORIGIN_CENTERRIGHT: {
            return (Vector2){1.0, 0.5};
            break;
        }
        case boshi_ORIGIN_CENTER: {
            return (Vector2){0.5, 0.5};
            break;
        }
        case boshi_ORIGIN_BOTTOMLEFT: {
            return (Vector2){0.0, 1.0};
            break;
        }
        case boshi_ORIGIN_BOTTOMRIGHT: {
            return (Vector2){1.0, 1.0};
            break;
        }
        case boshi_ORIGIN_BOTTOMCENTER: {
            return (Vector2){0.5, 1.0};
            break;
        }
        default: {
            return (Vector2){0.0, 0.0};
            break;
        }
    }
}

Texture2D boshi_core_LoadMissingTexture() {
    Image image = LoadImageFromMemory(".png", boshi_MissingTexture, boshi_MissingTextureLength);
    Texture2D tex = LoadTextureFromImage(image);
    UnloadImage(image);

    return tex;
}

void boshi_core_Quit() {
    printf("[boshi] Quitting...\n");
    UnloadRenderTexture(boshi_GAME_TEXTURE);
    CloseWindow();

    boshi_INITIALIZED = false;
}