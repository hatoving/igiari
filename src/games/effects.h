#ifndef IGIARI_TRILOGY_EFFECTS_H
#define IGIARI_TRILOGY_EFFECTS_H

typedef struct {
	int min;
	int max;
} igiari_trilogy_effects_ShakeParams;

typedef enum {
	IGIARI_TRILOGY_EFFECTS_SHAKE,
	IGIARI_TRILOGY_EFFECTS_FLASH
} igiari_trilogy_effects;

void igiari_trilogy_effects_Update();
void igiari_trilogy_effects_Render();

void igiari_trilogy_effects_Perform(igiari_trilogy_effects effect);

#endif