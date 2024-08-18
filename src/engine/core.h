#ifndef IGIARI_ENGINE_CORE_H
#define IGIARI_ENGINE_CORE_H

#include <SDL2/SDL.h>
#include "../utils/glad.h"

extern SDL_Window* IGIARI_ENGINE_SDLWINDOW;
extern SDL_GLContext IGIARI_ENGINE_GLCONTEXT;

extern int IGIARI_ENGINE_TARGETSIZE_WIDTH;
extern int IGIARI_ENGINE_TARGETSIZE_HEIGHT;

extern int IGIARI_ENGINE_RUNNING;
extern int IGIARI_ENGINE_FULLSCREEN;

void igiari_engine_core_Initialize(int window_width, int window_height, char* title);

void igiari_engine_core_StartUpdate(SDL_Event* e);

void igiari_engine_core_StartRender();
void igiari_engine_core_EndRender();

void igiari_engine_core_Quit();

#endif