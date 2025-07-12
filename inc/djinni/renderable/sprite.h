#ifndef DJINNI_RENDERABLE_SPRITE
#define DJINNI_RENDERABLE_SPRITE 1

#include "djinni/renderable/shared.h"
#include "djinni/renderable/entity.h"

struct Djinni_Renderable_SpriteStruct {
  Entity* (*create)(int, int, char*);
};

extern struct Djinni_Renderable_SpriteStruct Djinni_Renderable_Sprite;

#endif