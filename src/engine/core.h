#ifndef IGIARI_ENGINE_CORE_H
#define IGIARI_ENGINE_CORE_H

#include <SDL2/SDL.h>
#include "../utils/glad.h"

extern SDL_Window* IGIARI_ENGINE_SDLWINDOW;
extern SDL_GLContext IGIARI_ENGINE_GLCONTEXT;

extern int IGIARI_ENGINE_TARGETSIZE_WIDTH;
extern int IGIARI_ENGINE_TARGETSIZE_HEIGHT;

extern int IGIARI_ENGINE_WINDOW_WIDTH;
extern int IGIARI_ENGINE_WINDOW_HEIGHT;

extern float IGIARI_ENGINE_VIEWPORT_OFFSET_X;
extern float IGIARI_ENGINE_VIEWPORT_OFFSET_Y;
extern float IGIARI_ENGINE_VIEWPORT_WIDTH;
extern float IGIARI_ENGINE_VIEWPORT_HEIGHT;

extern int IGIARI_ENGINE_RUNNING;
extern int IGIARI_ENGINE_FULLSCREEN;

extern int IGIARI_ENGINE_FPS;
extern float IGIARI_ENGINE_CURRENT_FPS;
extern float IGIARI_ENGINE_DELTA_TIME;
extern float IGIARI_ENGINE_FRAME_START;
extern float IGIARI_ENGINE_FRAME_END;
extern double IGIARI_ENGINE_FRAME_DURATION;

void igiari_engine_core_Initialize(char* title, int window_width, int window_height, int target_width, int target_height, int fps);

void igiari_engine_core_StartUpdate(SDL_Event* e);
void igiari_engine_core_EndUpdate();

void igiari_engine_core_StartRender();
void igiari_engine_core_EndRender();

void igiari_engine_core_Quit();

#endif