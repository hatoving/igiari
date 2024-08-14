#include <string.h>

#include "engine/core.h"
#include "engine/object.h"
#include "engine/sprite.h"
#include "engine/unicode.h"

#include "mdt.h"

#include "unity/serialized_file.h"
#include "unity/bundle.h"

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

    igiari_unity_bundle bundle = igiari_unity_bundle_Read("titlegs1u.unity3d");
    igiari_unity_serizaliedfile file =  igiari_unity_serizaliedfile_ReadFromData(bundle.uncompressed_data, bundle.directory_info[0].offset);
    
    //unsigned char* tex2d = malloc((bundle.uncompressed_data_len - bundle.directory_info[1].offset));
    //const char* ptr = bundle.uncompressed_data + bundle.directory_info[1].offset;

    /*memcpy(tex2d, ptr, (bundle.uncompressed_data_len - bundle.directory_info[1].offset));

    FILE* f = fopen("tex2d.bin", "wb");
    fwrite(tex2d, (bundle.uncompressed_data_len - bundle.directory_info[1].offset), 1, f);
    fclose(f);

    printf("class type: %i\n", (bundle.uncompressed_data_len - bundle.directory_info[1].offset));*/

    int data_size = 0;
    uint8_t* data = LoadFileData("tex2d.bin", &data_size);

    Image image = {
        0,
        (int)1482,
        (int)706,
        1,
        PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };
    image.data = RL_MALLOC(data_size);
    memcpy(image.data, data, data_size);

    ImageFlipVertical(&image);

    Texture2D tex = LoadTextureFromImage(image);
    UnloadImage(image);
    UnloadFileData(data);

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
            DrawTextEx(font, mdt.operations[115].text, (Vector2){20, 20}, 30, 2, WHITE);
            DrawTexture(tex, 20, 20, WHITE);
        boshi_core_EndDrawing();
    }
    //UnloadTexture(texture);+
    UnloadFont(font);
    boshi_core_Quit();
    return 0;
}