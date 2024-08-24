#include <SDL2/SDL.h>

#include <stdio.h>
#include <math.h>

#include "../engine/core.h"
#include "../engine/imgui.h"

#include "effects.h"

SDL_TimerID shake_timer;
SDL_TimerID flash_timer;

int igiari_trilogy_effects_AllowFlash = 0;
igiari_trilogy_effects_ShakeParams shake_params;

Uint32 igiari_trilogy_effects_TimerStopShake(Uint32 interval, void *param) {
    shake_params.min = shake_params.max = 0;
    return interval;
}

Uint32 igiari_trilogy_effects_TimerStopFlash(Uint32 interval, void *param) {
    igiari_trilogy_effects_AllowFlash = 0;
    return interval;
}

void igiari_trilogy_effects_Update() {
	IGIARI_ENGINE_CAMERA2D->x = shake_params.min + rand() % (shake_params.max - shake_params.min + 1);
    IGIARI_ENGINE_CAMERA2D->y = shake_params.min + rand() % (shake_params.max - shake_params.min + 1);
}

void igiari_trilogy_effects_Render() {
	float scale_x = ((float)IGIARI_ENGINE_VIEWPORT_WIDTH / (float)IGIARI_ENGINE_TARGETSIZE_WIDTH);
    float scale_y = ((float)IGIARI_ENGINE_VIEWPORT_HEIGHT / (float)IGIARI_ENGINE_TARGETSIZE_HEIGHT);
	
	ImDrawList* list = igiari_imgui_GetWindowDrawList();
	
    //igiari_imgui_Begin("##trilogy_effects", NULL, igiari_imgui_WindowFlags_NoTitleBar | igiari_imgui_WindowFlags_NoResize | igiari_imgui_WindowFlags_NoScrollbar | igiari_imgui_WindowFlags_NoMove | igiari_imgui_WindowFlags_NoCollapse);
	if (igiari_trilogy_effects_AllowFlash == 1) {
		//printf("YES\n");
		igiari_imgui_AddRectFilledToDrawList(
			list,
			IGIARI_ENGINE_VIEWPORT_OFFSET_X, IGIARI_ENGINE_VIEWPORT_OFFSET_Y,
			(1980.0f * scale_x) + IGIARI_ENGINE_VIEWPORT_OFFSET_X, (1080.0f * scale_y) + IGIARI_ENGINE_VIEWPORT_OFFSET_Y,
			igiari_imgui_COL32(255, 255, 255, 255),
			0.0f, 0
		);
	}
}

void igiari_trilogy_effects_Perform(igiari_trilogy_effects effect) {
	switch (effect) {
		case IGIARI_TRILOGY_EFFECTS_SHAKE: {
            shake_params.min = -12; shake_params.max = 12;
			SDL_RemoveTimer(shake_timer);
            shake_timer = SDL_AddTimer(250, igiari_trilogy_effects_TimerStopShake, NULL);
			break;
		}
		case IGIARI_TRILOGY_EFFECTS_FLASH: {
			igiari_trilogy_effects_AllowFlash = 1;
			SDL_RemoveTimer(flash_timer);
			flash_timer = SDL_AddTimer(20, igiari_trilogy_effects_TimerStopFlash, NULL);
			break;
		}
	}
}