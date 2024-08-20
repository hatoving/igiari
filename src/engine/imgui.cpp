#include "../utils/glad.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

#include <cstdarg>
#include <cstdio>

#include "imgui.h"

void igiari_imgui_CreateContext(SDL_Window* window, SDL_GLContext ctx) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.WindowRounding = 1.0f;
	style.FrameRounding = 5.0f;
	style.ItemSpacing.y = 16.0f;
	style.ItemSpacing.x = 12.0f;
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	ImGui::StyleColorsClassic();
	
	ImFont* default_f = io.Fonts->AddFontDefault();
	default_f->Scale = 1.5f;
	
	ImGui_ImplSDL2_InitForOpenGL(window, ctx);
	ImGui_ImplOpenGL3_Init("#version 100");
}

void igiari_imgui_NewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void igiari_imgui_Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool igiari_imgui_Begin(const char* name, bool* p_open, int flags) {
	return ImGui::Begin(name, p_open, (ImGuiWindowFlags)flags);
}

void igiari_imgui_SetGlobalFontScale(float scale) {
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = scale;
}

float igiari_imgui_GetGlobalFontScale() {
	ImGuiIO& io = ImGui::GetIO();
	return io.FontGlobalScale;
}

void igiari_imgui_FontCalcTextSizeA(ImFont* font, char* text, float size, float wrap_width, float* width, float* height) {
	ImVec2 s = font->CalcTextSizeA(size, FLT_MAX, wrap_width, text);
	*width = s.x; *height = s.y;
}

float igiari_imgui_ManualTextWidth(const char* text) {
    ImFont* font = ImGui::GetFont();
    float fontScale = font->Scale;
    float width = 0.0f;

    const ImFontGlyph* glyph = nullptr;

    for (const char* p = text; *p; ++p) {
        glyph = font->FindGlyph(*p);
        if (glyph) {
            width += glyph->AdvanceX;
        }
    }

    width *= fontScale;
    return width;
}

void igiari_imgui_SetNextWindowBgAlpha(float alpha) {
	ImGui::SetNextWindowBgAlpha(alpha);
}
void igiari_imgui_SetNextWindowPos(float x, float y, int cond, float pivot_x, float pivot_y) {
	ImGui::SetNextWindowPos(ImVec2(x, y), cond, ImVec2(pivot_x, pivot_y));
}

void igiari_imgui_SetNextWindowSize(float x, float y, int cond) {
	ImGui::SetNextWindowSize(ImVec2(x, y), cond);
}

void igiari_imgui_End() {
	ImGui::End();
}

void igiari_imgui_ShowDemoWindow() {
	ImGui::ShowDemoWindow();
}

bool igiari_imgui_sdl2_ProcessEvent(const SDL_Event* event) {
	return ImGui_ImplSDL2_ProcessEvent(event);
}

void igiari_imgui_SetCursorPos(float x, float y) {
	ImGui::SetCursorPos(ImVec2(x, y));
}

void igiari_imgui_SetCursorPosX(float x) {
	ImGui::SetCursorPosX(x);
}

void igiari_imgui_SetCursorPosY(float y) {
	ImGui::SetCursorPosY(y);
}

void igiari_imgui_Text(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	ImGui::TextV(fmt, args);
	va_end(args);
}


void igiari_imgui_TextUnformatted(const char* fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    ImGui::TextUnformatted(buffer);
}

void igiari_imgui_SameLine(float offset_from_start_x, float spacing) {
	ImGui::SameLine(offset_from_start_x, spacing);
}
void igiari_imgui_CalcTextSize(const char* label, const char* text_end, bool hide_text_after_double_hash, float wrap_width, float* width, float* height) {
	ImVec2 size = ImGui::CalcTextSize(label, text_end, hide_text_after_double_hash, wrap_width);
	*width = size.x; *height = size.y;
}

void igiari_imgui_PushFont(ImFont* font) {
	ImGui::PushFont(font);
}
void igiari_imgui_PopFont() {
	ImGui::PopFont();
}

void igiari_imgui_PushStyleColor(int idx, unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
	ImGui::PushStyleColor(idx, IM_COL32(r,g,b,a));
}
void igiari_imgui_PopStyleColor() {
	ImGui::PopStyleColor();
}

ImFont* igiari_imgui_GetDefaultFont() {
	return ImGui::GetIO().FontDefault;
}
ImFont* igiari_imgui_AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges) {
	ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, size_pixels, font_cfg, glyph_ranges);
	return font;
}

void igiari_imgui_ChangeFontScale(ImFont* font, float scale) {
	font->Scale = scale;
}

void igiari_imgui_Dummy(float x, float y) {
	ImGui::Dummy(ImVec2(x, y));
}

void igiari_imgui_DestroyContext() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}