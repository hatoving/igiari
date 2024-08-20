#ifndef IGIARI_ENGINE_IMGUI_WRAPPER_H
#define IGIARI_ENGINE_IMGUI_WRAPPER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ImVec2 ImVec2;
typedef struct ImFont ImFont;
typedef struct ImFontConfig ImFontConfig;
typedef unsigned int ImWchar32;
typedef unsigned short ImWchar16;
typedef ImWchar16 ImWchar;

struct ImFont;
struct ImFontConfig;

typedef enum {
    igiari_imgui_WindowFlags_None = 0,
    igiari_imgui_WindowFlags_NoTitleBar = 1 << 0,
    igiari_imgui_WindowFlags_NoResize = 1 << 1,
    igiari_imgui_WindowFlags_NoMove = 1 << 2,
    igiari_imgui_WindowFlags_NoScrollbar = 1 << 3,
    igiari_imgui_WindowFlags_NoScrollWithMouse = 1 << 4,
    igiari_imgui_WindowFlags_NoCollapse = 1 << 5,
    igiari_imgui_WindowFlags_AlwaysAutoResize = 1 << 6,
    igiari_imgui_WindowFlags_NoBackground = 1 << 7,
    igiari_imgui_WindowFlags_NoSavedSettings = 1 << 8,
    igiari_imgui_WindowFlags_NoMouseInputs = 1 << 9,
    igiari_imgui_WindowFlags_MenuBar = 1 << 10,
    igiari_imgui_WindowFlags_HorizontalScrollbar = 1 << 11,
    igiari_imgui_WindowFlags_NoFocusOnAppearing = 1 << 12,
    igiari_imgui_WindowFlags_NoBringToFrontOnFocus = 1 << 13,
    igiari_imgui_WindowFlags_AlwaysVerticalScrollbar= 1 << 14,
    igiari_imgui_WindowFlags_AlwaysHorizontalScrollbar=1<< 15,
    igiari_imgui_WindowFlags_NoNavInputs = 1 << 16,
    igiari_imgui_WindowFlags_NoNavFocus = 1 << 17,
    igiari_imgui_WindowFlags_UnsavedDocument = 1 << 18,
    igiari_imgui_WindowFlags_NoDocking = 1 << 19,
    igiari_imgui_WindowFlags_NoNav = igiari_imgui_WindowFlags_NoNavInputs | igiari_imgui_WindowFlags_NoNavFocus,
    igiari_imgui_WindowFlags_NoDecoration = igiari_imgui_WindowFlags_NoTitleBar | igiari_imgui_WindowFlags_NoResize | igiari_imgui_WindowFlags_NoScrollbar | igiari_imgui_WindowFlags_NoCollapse,
    igiari_imgui_WindowFlags_NoInputs = igiari_imgui_WindowFlags_NoMouseInputs | igiari_imgui_WindowFlags_NoNavInputs | igiari_imgui_WindowFlags_NoNavFocus,
    igiari_imgui_WindowFlags_ChildWindow = 1 << 24,
    igiari_imgui_WindowFlags_Tooltip = 1 << 25,
    igiari_imgui_WindowFlags_Popup = 1 << 26,
    igiari_imgui_WindowFlags_Modal = 1 << 27,
    igiari_imgui_WindowFlags_ChildMenu = 1 << 28,
    igiari_imgui_WindowFlags_DockNodeHost = 1 << 29,
}igiari_imgui_WindowFlags;

void igiari_imgui_CreateContext(SDL_Window* window, SDL_GLContext ctx);
void igiari_imgui_DestroyContext();

void igiari_imgui_NewFrame();
void igiari_imgui_Render();

bool igiari_imgui_Begin(const char* name, bool* p_open, int flags);
void igiari_imgui_End();

bool igiari_imgui_sdl2_ProcessEvent(const SDL_Event* event);
void igiari_imgui_ShowDemoWindow();

void igiari_imgui_SetGlobalFontScale(float scale);
float igiari_imgui_GetGlobalFontScale();

void igiari_imgui_SetNextWindowBgAlpha(float alpha);
void igiari_imgui_SetNextWindowPos(float x, float y, int cond, float pivot_x, float pivot_y);
void igiari_imgui_SetNextWindowSize(float x, float y, int cond);

void igiari_imgui_Text(const char* fmt, ...);
void igiari_imgui_TextUnformatted(const char* fmt, ...);
void igiari_imgui_SameLine(float offset_from_start_x, float spacing);
void igiari_imgui_CalcTextSize(const char* label, const char* text_end, bool hide_text_after_double_hash, float wrap_width, float* width, float* height);

void igiari_imgui_PushStyleColor(int idx, unsigned int r, unsigned int g, unsigned int b, unsigned int a);
void igiari_imgui_PopStyleColor();

void igiari_imgui_PushFont(ImFont* font);
void igiari_imgui_ChangeFontScale(ImFont* font, float scale);
void igiari_imgui_FontCalcTextSizeA(ImFont* font, char* text, float size, float wrap_width, float* width, float* height);
void igiari_imgui_PopFont();

float igiari_imgui_ManualTextWidth(const char* text);

void igiari_imgui_Dummy(float x, float y);

ImFont* igiari_imgui_GetDefaultFont();

void igiari_imgui_SetCursorPos(float x, float y); 
void igiari_imgui_SetCursorPosX(float x);
void igiari_imgui_SetCursorPosY(float y); 

ImFont* igiari_imgui_AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges);

#ifdef __cplusplus
}
#endif

#endif
