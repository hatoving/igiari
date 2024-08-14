#include "sprite.h"

boshi_sprite* boshi_sprite_Create(Vector2 pos, Texture2D tex) {
    boshi_sprite* spr = MemAlloc(sizeof(boshi_sprite));

    spr->base_object = boshi_object_Create(pos);
    spr->texture = tex;

    return spr;
}

void boshi_sprite_Draw(boshi_sprite* spr) {
    Vector2 origin;
    if (spr->base_object->origin != boshi_ORIGIN_CUSTOM)
        origin = boshi_core_GetOrigin(spr->base_object->origin);
    else
        origin = spr->base_object->actual_origin;

    DrawTexturePro(
        spr->texture,
        (Rectangle){0, 0, spr->texture.width, spr->texture.height},
        (Rectangle){spr->base_object->position.x, spr->base_object->position.y, spr->texture.width * spr->base_object->scale.x, spr->texture.height * spr->base_object->scale.y},
        (Vector2){(spr->texture.width * origin.x) * spr->base_object->scale.x,(spr->texture.height * origin.y) * spr->base_object->scale.y},
        spr->base_object->rotation,
        WHITE
    );
}