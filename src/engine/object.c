#include "core.h"
#include "object.h"

boshi_object* boshi_object_Create(Vector2 pos) {
    boshi_object* obj = MemAlloc(sizeof(boshi_object));

    obj->position = pos;
    obj->scale = (Vector2){1.0, 1.0};
    obj->rotation = 0.0;

    obj->origin = boshi_ORIGIN_CENTER;
    obj->actual_origin = boshi_core_GetOrigin(obj->origin);

    return obj;
}

void boshi_object_SetOrigin(boshi_object* obj, boshi_core_Origin origin) {
    obj->origin = origin;
}

void boshi_object_SetCustomOrigin(boshi_object* obj, Vector2 origin) {
    obj->origin = boshi_ORIGIN_CUSTOM;
    obj->actual_origin = origin;
}

/*void boshi_object_Draw(boshi_object* obj) {
    DrawTexturePro(
        obj->texture,
        (Rectangle){0, 0, obj->texture.width, obj->texture.height},
        (Rectangle){obj->position.x, obj->position.y, obj->texture.width * obj->scale.x, obj->texture.height * obj->scale.y},
        (Vector2){(obj->texture.width * boshi_core_GetOrigin(obj->origin).x) * obj->scale.x, (obj->texture.height * boshi_core_GetOrigin(obj->origin).y) * obj->scale.y},
        obj->rotation, WHITE
    );
}*/