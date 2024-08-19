#include "../utils/glad.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

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
	
	io.Fonts->AddFontDefault();
	
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

void igiari_imgui_Text(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	ImGui::TextV(fmt, args);
	va_end(args);
}
void igiari_imgui_SameLine(float offset_from_start_x, float spacing) {
	ImGui::SameLine(offset_from_start_x, spacing);
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

void igiari_imgui_DestroyContext() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}