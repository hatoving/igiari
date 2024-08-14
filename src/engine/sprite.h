#ifndef boshi_SPRITE_H
#define boshi_SPRITE_H

#include "object.h"

typedef struct {
    boshi_object* base_object;
    Texture2D texture;
} boshi_sprite;
boshi_sprite* boshi_sprite_Create(Vector2 pos, Texture2D texture);
void boshi_sprite_Draw(boshi_sprite* spr);

#endif