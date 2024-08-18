#include "core.h"

#include <stdio.h>

SDL_Window* IGIARI_ENGINE_SDLWINDOW = NULL;
SDL_GLContext IGIARI_ENGINE_GLCONTEXT = {0};

int IGIARI_ENGINE_TARGETSIZE_WIDTH = 1980;
int IGIARI_ENGINE_TARGETSIZE_HEIGHT = 1080;

int IGIARI_ENGINE_RUNNING = 0;
int IGIARI_ENGINE_FULLSCREEN = 0;

void igiari_engine_core_Initialize(int window_width, int window_height, char* title) {
    printf("[igiari, engine] Starting engine...\n");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Init(SDL_INIT_VIDEO);
    IGIARI_ENGINE_SDLWINDOW = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    IGIARI_ENGINE_GLCONTEXT = SDL_GL_CreateContext(IGIARI_ENGINE_SDLWINDOW);
    SDL_GL_MakeCurrent(IGIARI_ENGINE_SDLWINDOW, IGIARI_ENGINE_GLCONTEXT);

    gladLoadGLES2Loader(SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    IGIARI_ENGINE_RUNNING = 1;
    IGIARI_ENGINE_FULLSCREEN = 0;
}

void igiari_engine_core_StartUpdate(SDL_Event* e) {
    while(SDL_PollEvent(e)) {
        if(e->type == SDL_QUIT)
            IGIARI_ENGINE_RUNNING = 0;
        if (e->type == SDL_WINDOWEVENT) {
            if (e->window.event == SDL_WINDOWEVENT_RESIZED) {
                int width = e->window.data1;
                int height = e->window.data2;

                float target_aspect_ratio = (float)IGIARI_ENGINE_TARGETSIZE_WIDTH / (float)IGIARI_ENGINE_TARGETSIZE_HEIGHT;
                float screen_aspect_ratio = (float)width / (float)height;  // Make sure this is a float division

                float viewport_width, viewport_height;
                float viewport_offset_x = 0.0f, viewport_offset_y = 0.0f;

                if (screen_aspect_ratio > target_aspect_ratio) {
                    // Letterbox vertically
                    viewport_height = (float)height;
                    viewport_width = viewport_height * target_aspect_ratio;
                    viewport_offset_x = (float)(width - viewport_width) / 2.0f;
                } else {
                    // Letterbox horizontally
                    viewport_width = (float)width;
                    viewport_height = viewport_width / target_aspect_ratio;
                    viewport_offset_y = (float)(height - viewport_height) / 2.0f;
                }

                glViewport((GLint)viewport_offset_x, (GLint)viewport_offset_y, (GLint)viewport_width, (GLint)viewport_height);
            }
        }
        if (e->type == SDL_KEYDOWN) {
            if (e->key.keysym.sym == SDLK_F11) {
                if (IGIARI_ENGINE_FULLSCREEN) {
                    SDL_SetWindowFullscreen(IGIARI_ENGINE_SDLWINDOW, 0);
                    IGIARI_ENGINE_FULLSCREEN = 0;
                } else {
                    SDL_SetWindowFullscreen(IGIARI_ENGINE_SDLWINDOW, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    IGIARI_ENGINE_FULLSCREEN = 1;
                }
            }
        }
    }
}

void igiari_engine_core_StartRender() {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void igiari_engine_core_EndRender() {
    SDL_GL_SwapWindow(IGIARI_ENGINE_SDLWINDOW);
}

void igiari_engine_core_Quit() {
    printf("[igiari, engine] Quitting engine...\n");

    SDL_GL_MakeCurrent(IGIARI_ENGINE_SDLWINDOW, NULL);
    SDL_GL_DeleteContext(IGIARI_ENGINE_GLCONTEXT);
    SDL_DestroyWindow(IGIARI_ENGINE_SDLWINDOW);

    SDL_Quit();
}