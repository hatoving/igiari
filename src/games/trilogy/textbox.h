#ifndef IGIARI_TRILOGY_TEXTBOX_H
#define IGIARI_TRILOGY_TEXTBOX_H

#include "../../assets/pwaat/mdt.h"
#include "../../engine/imgui.h"
#include "sound_data.h"

typedef enum {
	IGIARI_TRILOGY_TEXTBOX_BREAK = 1,
	IGIARI_TRILOGY_TEXTBOX_MUSIC = 5,
	IGIARI_TRILOGY_TEXTBOX_SPEED = 11,
	IGIARI_TRILOGY_TEXTBOX_WAIT = 12,
	IGIARI_TRILOGY_TEXTBOX_HIDE = 46,
} igiari_trilogy_textbox_opcodes;

typedef enum {
	IGIARI_TRILOGY_TEXTBOX_WHITE,
	IGIARI_TRILOGY_TEXTBOX_RED,
} igiari_trilogy_textbox_colors;
typedef struct {
	char* character;
	igiari_trilogy_textbox_colors color;
} igiari_trilogy_textbox_msg;
typedef struct {
	int count;
	igiari_trilogy_textbox_msg* msgs;
} igiari_trilogy_textbox_msg_holder;

extern float IGIARI_TRILOGY_TEXTBOX_Y;
extern int IGIARI_TRILOGY_TEXTBOX_ENABLED;

void igiari_trilogy_textbox_Init(igiari_mdt_op* ops_to_execute, int size);
void igiari_trilogy_textbox_Update(float delta_time);
void igiari_trilogy_textbox_Render(ImFont* font);

#endif