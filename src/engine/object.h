#ifndef boshi_OBJECT_H
#define boshi_OBJECT_H

#include "core.h"

#include <stdio.h>
#include <raylib.h>

typedef struct {
    Vector2 position;
    Vector2 scale;
    float rotation;

    boshi_core_Origin origin;
    Vector2 actual_origin;
} boshi_object;
boshi_object* boshi_object_Create(Vector2 pos);

void boshi_object_SetOrigin(boshi_object* obj, boshi_core_Origin origin);
void boshi_object_SetCustomOrigin(boshi_object* obj, Vector2 origin);

#endif