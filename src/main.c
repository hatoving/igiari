#include <string.h>

#include "engine/core.h"
#include "engine/object.h"
#include "engine/sprite.h"
#include "engine/unicode.h"

#include "mdt.h"

#define RLUNICODE_IMPLEMENTATION 1
#include "rlunicode.h"

#include <malloc.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
    if (boshi_core_Initialize("igiari", 1280, 720, 1280, 720, 60, true) != 0) {
        printf("[igiari] Failed to initiliate engine.\n");
        return 1;
    } else {
        boshi_core_SetRenderMode(boshi_RENDER_MODE_FIT);
    }

    igiari_mdt mdt = igiari_mdt_Read("ev0_mes_u.mdt.dec");
    Font font = LoadFont("FOT-ModeMinBLargeStd-R.otf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    while(!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) {
            ToggleBorderlessWindowed();
        }
        
        boshi_core_BeginDrawing();
            ClearBackground(BLACK);
            //DrawTextCodepoint(font, GetCodepointFromUTF16(&mdt.operations[20].text[1], &wordsProcessed), (Vector2){20, 20}, 20, WHITE);
            //DrawText(mdt.operations[274].text, 20, 20, 16, WHITE);
            DrawTextEx(font, mdt.operations[86].text, (Vector2){20, 20}, 20, 2, WHITE);
        boshi_core_EndDrawing();
    }
    //UnloadTexture(texture);+
    UnloadFont(font);
    boshi_core_Quit();
    return 0;
}