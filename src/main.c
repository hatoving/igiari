#include <stdio.h>
#include <string.h>

#include "mdt.h"

#include "unity/serialized_file.h"
#include "unity/bundle.h"
#include "unity/audio_clip.h"
#include "unity/texture2d.h"

#include "fmod/rebuild.h"

#include <malloc.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include "utils/glad.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc,char **argv) {
    SDL_Window* window = NULL;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("igiari", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

    SDL_GLContext ctx = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, ctx);

    gladLoadGLES2Loader(SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);

    igiari_unity_bundle* gs1_logo_bundle = igiari_unity_bundle_Read("titlegs1u.unity3d");
    int game_bg_tex_count = 0;

    igiari_unity_texture2d* gs1_logo_texarray = igiari_unity_texture2d_GetAllTexFromNode(gs1_logo_bundle, "CAB-1f36da66d6416727fb8d0b18cb649fae", &game_bg_tex_count);
    igiari_unity_texture2d* gs1_logo_tex = igiari_unity_texture2d_GetTexByName(gs1_logo_texarray, game_bg_tex_count, "titlegs1u");

    const float vertices[] = {
        0.75f, 0.75f, 0.0f, 1.0f,
        0.75f, -0.75f, 0.0f, 1.0f,
        -0.75f, -0.75f, 0.0f, 1.0f,
    };

    SDL_Event e; bool quit = false;
    while(quit == false)
    {
        while(SDL_PollEvent(&e)) {
            if( e.type == SDL_QUIT )
                quit = true;
        }

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);


        SDL_GL_SwapWindow(window);
    }

    SDL_GL_MakeCurrent(window, NULL);
    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}