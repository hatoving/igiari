#include "core.h"

#include <stdio.h>

SDL_Window* IGIARI_ENGINE_SDLWINDOW = NULL;
SDL_GLContext IGIARI_ENGINE_GLCONTEXT = {0};

int IGIARI_ENGINE_TARGETSIZE_WIDTH = 1980;
int IGIARI_ENGINE_TARGETSIZE_HEIGHT = 1080;

int IGIARI_ENGINE_RUNNING = 0;
int IGIARI_ENGINE_FULLSCREEN = 0;

void igiari_engine_core_impl_Letterbox(SDL_Event* e) {
    int width = e->window.data1;
    int height = e->window.data2;

    float target_aspect_ratio = IGIARI_ENGINE_TARGETSIZE_WIDTH / IGIARI_ENGINE_TARGETSIZE_HEIGHT;
    float screen_aspect_ratio = width / height;

    float viewport_width, viewport_height;
    float viewport_offset_x = 0.0f, viewport_offset_y = 0.0f;

    if (screen_aspect_ratio > target_aspect_ratio) {
        viewport_height = width;
        viewport_width = viewport_height * target_aspect_ratio;
        viewport_offset_x = (width - viewport_width) / 2.0f;
    } else {
        viewport_width = width;
        viewport_height = viewport_width / target_aspect_ratio;
        viewport_offset_y = (height - viewport_height) / 2.0f;
    }
}

void igiari_engine_core_Initialize(int window_width, int window_height, char* title) {
    printf("[igiari, engine] Starting engine...\n");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Init(SDL_INIT_VIDEO);
    IGIARI_ENGINE_SDLWINDOW = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

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
                igiari_engine_core_impl_Letterbox(e);
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