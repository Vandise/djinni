#ifndef DJINNI_RENDERABLE
#define DJINNI_RENDERABLE 1

#include "djinni/renderable/shared.h"
#include "djinni/renderable/entity.h"
#include "djinni/renderable/sprite.h"

struct Djinni_RenderableStruct {
  struct Djinni_Renderable_EntityStruct* Entity;
  struct Djinni_Renderable_SpriteStruct* Sprite;

  void (*initialize)();
};

extern struct Djinni_RenderableStruct Djinni_Renderable;

#endif