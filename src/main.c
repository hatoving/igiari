#include <string.h>

#include "engine/core.h"
#include "engine/object.h"
#include "engine/sprite.h"
#include "engine/unicode.h"

#include "mdt.h"

#include "unity/serialized_file.h"
#include "unity/bundle.h"
#include "unity/texture2d.h"

#define RLUNICODE_IMPLEMENTATION 1
#include "rlunicode.h"

#include <malloc.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
    if (boshi_core_Initialize("igiari", 960, 544, 1280, 720, 60, true) != 0) {
        printf("[igiari] Failed to initiliate engine.\n");
        return 1;
    } else {
        boshi_core_SetRenderMode(boshi_RENDER_MODE_FIT);
    }

    igiari_mdt mdt = igiari_mdt_Read("ev0_mes_u.mdt.dec");

    igiari_unity_bundle bundle = igiari_unity_bundle_Read("thumb1bt.unity3d");
    int tex_count = 0;

    igiari_unity_texture2d* tex_array = igiari_unity_texture2d_GetAllTexture2DsFromNode(&bundle, "CAB-90678161265c4679c0d704b633bd4504", &tex_count);
    igiari_unity_texture2d* tex_unity = &tex_array[0];

    Texture2D tex = igiari_unity_texture2d_ConvertIntoRaylibTexture2D(tex_unity);
    //igiari_unity_serizaliedfile file =  igiari_unity_serizaliedfile_ReadFromData(bundle.uncompressed_data, igiari_unity_bundle_GetNodeByPath(&bundle, "CAB-1f36da66d6416727fb8d0b18cb649fae")->offset);

    /*unsigned char* tex2d = malloc(file.object_infos[2].byte_size);
    const char* ptr = bundle.uncompressed_data + file.object_infos[2].byte_start;

    memcpy(tex2d, ptr, file.object_infos[2].byte_size);

    FILE* f = fopen("texmetadata.bin", "wb");
    fwrite(tex2d, file.object_infos[2].byte_size, 1, f);
    fclose(f);*/

    //printf("Tex Size: %s\n", igiari_unity_bundle_GetNodeByPath(&bundle, "CAB-1f36da66d6416727fb8d0b18cb649fae")->offset);
    
    //unsigned char* tex2d = malloc((bundle.uncompressed_data_len - bundle.directory_info[1].offset));
    //const char* ptr = bundle.uncompressed_data + bundle.directory_info[1].offset;

    /*memcpy(tex2d, ptr, (bundle.uncompressed_data_len - bundle.directory_info[1].offset));

    FILE* f = fopen("tex2d.bin", "wb");
    fwrite(tex2d, (bundle.uncompressed_data_len - bundle.directory_info[1].offset), 1, f);
    fclose(f);

    printf("class type: %i\n", (bundle.uncompressed_data_len - bundle.directory_info[1].offset));*/

    /*int data_size = 0;
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
    UnloadFileData(data);*/

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
            DrawTexturePro(tex, (Rectangle){0, 0, tex.width, -tex.height}, (Rectangle){0, 0, tex.width, tex.height}, (Vector2){0, 0}, 0.0, WHITE);
        boshi_core_EndDrawing();
    }
    //UnloadTexture(texture);+
    UnloadFont(font);
    boshi_core_Quit();
    return 0;
}