#include "textbox.h"
#include "../effects.h"

#include "../../utils/unicode.h"
#include "../../engine/core.h"

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>

float IGIARI_TRILOGY_TEXTBOX_Y = 340;

igiari_trilogy_textbox_msg_holder** current_msgs;
int msg_count = 0;
int msg_index = 0;

igiari_mdt_op* ops;
int op_count = 3;

int current_op = 0;
int perform_op = 0;
int done = 0;

int speed = 0;

float textbox_wait_timer = 0.0f;
float textbox_speed_timer = 0.0f;

int IGIARI_TRILOGY_TEXTBOX_ENABLED = 0;

void igiari_trilogy_textbox_Init(igiari_mdt_op* ops_to_execute, int size) {
	ops = ops_to_execute;
	op_count = size;
	
	perform_op = 1;
	done = 0;
	
	IGIARI_TRILOGY_TEXTBOX_ENABLED = 1;
}

void igiari_trilogy_textbox_AddChar(char* character) {
	
}

void igiari_trilogy_textbox_Update(float delta_time) {
	if (IGIARI_TRILOGY_TEXTBOX_ENABLED) {
		if (textbox_wait_timer > 0.0f) {
			textbox_wait_timer -= delta_time;
			if (textbox_wait_timer < 0.0f) {
				textbox_wait_timer = 0.0f;
			}
		} else if (textbox_wait_timer <= 0.0f) {
			textbox_wait_timer = 0.0f;
			if (perform_op == 0 && textbox_speed_timer <= 0.0f) perform_op = 1;
		}

		if (textbox_speed_timer > 0.0f) {
			textbox_speed_timer -= delta_time;
			if (textbox_speed_timer < 0.0f) {
				textbox_speed_timer = 0.0f;
			}
		} else if (textbox_speed_timer <= 0.0f) {
			textbox_speed_timer = 0.0f;
			if (perform_op == 0 && textbox_wait_timer <= 0.0f) perform_op = 1;
		}

		if (ops != NULL && perform_op) {
			switch(ops[current_op].op) {
				case (int)IGIARI_TRILOGY_TEXTBOX_WAIT: {
					printf("[igiari, trilogy] Waiting for %.2f seconds...\n", (float)ops[current_op].args[0] / 60.0f);
					
					//SDL_RemoveTimer(textbox_wait_timer);
				//	textbox_wait_timer = SDL_AddTimer(((float)ops[current_op].args[0] / 60.0f) * 1000.0f, igiari_trilogy_effects_ResumeTextbox, NULL);
					textbox_wait_timer = ((float)ops[current_op].args[0] / 60.0f);
					perform_op = 0;
					
					break;
				}
				case (int)IGIARI_TRILOGY_TEXTBOX_MUSIC: {
					printf("[igiari, trilogy] Playing BGM: %i\n", ops[current_op].args[0]);
					Mix_PlayMusicStream(igiari_trilogy_GetBGM(ops[current_op].args[0])->src, -1);
					break;
				}
				case (int)IGIARI_TRILOGY_TEXTBOX_SPEED: {
					speed = ops[current_op].args[0];
					//printf("[igiari, trilogy] Playing BGM: %i\n", ops[current_op].args[0]);
					//Mix_PlayMusicStream(igiari_trilogy_GetBGM(ops[current_op].args[0])->src, -1);
					break;
				}
				case 18: {
					if (ops[current_op].args[0] == 769 && ops[current_op].args[1] == 8 && ops[current_op].args[2] == 31) {
						igiari_trilogy_effects_Perform(IGIARI_TRILOGY_EFFECTS_FLASH);
					}
					break;
				}
				case 39: {
					igiari_trilogy_effects_Perform(IGIARI_TRILOGY_EFFECTS_SHAKE);
					break;
				}
				case 0xFF: {
					perform_op = 0;
					switch (speed) {
						case 8: {
							textbox_speed_timer = 0.06;
							break;
						}
						case 6: {
							textbox_speed_timer = 0.05;
							break;
						}
						case 5: {
							textbox_speed_timer = 0.04;
							break;
						}
						case 4: {
							textbox_speed_timer = 0.025;
							break;
						}
						case 2: {
							textbox_speed_timer = 0.01;
							break;
						}
						default: {
							textbox_speed_timer = 0.01;
						}
					}
					//SDL_RemoveTimer(textbox_speed_timer);
					//textbox_speed_timer = SDL_AddTimer((1.0f) * 1000.0f, igiari_trilogy_effects_ResumeTextbox, ops[current_op].character);
					printf("[igiari, trilogy] \"%s\"\n", ops[current_op].character);
					break;
				}
				case 0xFF + 1: {
					perform_op = 0;
					done = 1;
					break;
				}
				default: {
					printf("[igiari, trilogy] Unknown op: %i\n", ops[current_op].op);
				}
			}
			current_op++;
		}
	}
}

void igiari_trilogy_textbox_Render(ImFont* font) {
	float scale_x = ((float)IGIARI_ENGINE_VIEWPORT_WIDTH / (float)IGIARI_ENGINE_TARGETSIZE_WIDTH);
    float scale_y = ((float)IGIARI_ENGINE_VIEWPORT_HEIGHT / (float)IGIARI_ENGINE_TARGETSIZE_HEIGHT);
				
	igiari_imgui_ChangeFontScale(font, 1.0f);   
	igiari_imgui_PushFont(font);
	igiari_imgui_SetCursorPos(
		(380.0f) * scale_x + IGIARI_ENGINE_VIEWPORT_OFFSET_X,
		(IGIARI_TRILOGY_TEXTBOX_Y + 450.0f) * scale_y + IGIARI_ENGINE_VIEWPORT_OFFSET_Y
	);
	igiari_imgui_PushStyleColor(0, 255, 255, 255, 255); igiari_imgui_Text("%.2f", textbox_wait_timer);
	//igiari_imgui_SameLine(0.0f, -1.0f); igiari_imgui_Image(test_button->texture->id, test_button->width * 1.5f, test_button->height * 1.5f);
	
	
	for (int i = 0; i < msg_count; i++) {
		igiari_imgui_SetCursorPos(
			(380.0f) * scale_x + IGIARI_ENGINE_VIEWPORT_OFFSET_X,
			(IGIARI_TRILOGY_TEXTBOX_Y + 450.0f + (65.0f * (i))) * scale_y + IGIARI_ENGINE_VIEWPORT_OFFSET_Y
		);
		for (int j = 0; j < (current_msgs[i])->count; j++) {
			char limitedText[256]; // Adjust size as needed
			//igiari_unicode_TruncateUTF8String(current_msgs[0], 8, limitedText);
			//printf("%s\n",(current_msgs[i])[j].msg);
			if (j != 0) igiari_imgui_SameLine(0.0f, 0.0f);
			switch ((current_msgs[i])->msgs[j].color) {
				case IGIARI_TRILOGY_TEXTBOX_WHITE: {
					igiari_imgui_PushStyleColor(0, 255, 255, 255, 255);
					break;
				}
				case IGIARI_TRILOGY_TEXTBOX_RED: {
					igiari_imgui_PushStyleColor(0, 255, 0, 0, 255);
					break;
				}
			}
			igiari_imgui_Text((current_msgs[i])->msgs[j].character);
			igiari_imgui_PopStyleColor();
		}
	}
	
	igiari_imgui_PopStyleColor(); igiari_imgui_PopFont();
	
	igiari_imgui_ChangeFontScale(font, 0.8f);
	igiari_imgui_PushFont(font);
	
	float text_width;
	text_width = igiari_imgui_ManualTextWidth("Phoenix");
	igiari_imgui_SetCursorPos(
		(312.0f - (text_width * 0.5) - IGIARI_ENGINE_CAMERA2D->x) * scale_x + IGIARI_ENGINE_VIEWPORT_OFFSET_X,
		(IGIARI_TRILOGY_TEXTBOX_Y + 389.0f - IGIARI_ENGINE_CAMERA2D->y) * scale_y + IGIARI_ENGINE_VIEWPORT_OFFSET_Y
	);
	
	igiari_imgui_TextUnformatted("Phoenix");
	igiari_imgui_PopFont();
	
	igiari_imgui_ChangeFontScale(font, 0.5f);
	igiari_imgui_PushFont(font);
	
	igiari_imgui_PushStyleColor(0, 0, 0, 0, 255);
	igiari_imgui_SetCursorPos(
		(1440.0f - IGIARI_ENGINE_CAMERA2D->x) * scale_x + IGIARI_ENGINE_VIEWPORT_OFFSET_X,
		(IGIARI_TRILOGY_TEXTBOX_Y + 647.0f - IGIARI_ENGINE_CAMERA2D->y) * scale_y + IGIARI_ENGINE_VIEWPORT_OFFSET_Y
	);
	igiari_imgui_Text("[Esc] Options");
	text_width = igiari_imgui_ManualTextWidth("[Tab] Court Record");
	igiari_imgui_SetCursorPos(
		(1980.0f - text_width - 85.0f - IGIARI_ENGINE_CAMERA2D->x) * scale_x + IGIARI_ENGINE_VIEWPORT_OFFSET_X,
		(IGIARI_TRILOGY_TEXTBOX_Y + 647.0f - IGIARI_ENGINE_CAMERA2D->y) * scale_y + IGIARI_ENGINE_VIEWPORT_OFFSET_Y
	);
	igiari_imgui_Text("[Tab] Court Record");
	igiari_imgui_PopStyleColor();
	
	igiari_imgui_PopFont();
	igiari_imgui_ChangeFontScale(font, 1.0f);
}