#ifndef DJINNI_RENDERABLE
#define DJINNI_RENDERABLE 1

#include "djinni/renderable/shared.h"
#include "djinni/video/video.h"
#include "djinni/game/camera.h"

#include "djinni/renderable/entity.h"
#include "djinni/renderable/sprite.h"
#include "djinni/renderable/shape/shape.h"
#include "djinni/renderable/paint.h"

struct Djinni_RenderableStruct {
  struct Djinni_Renderable_EntityStruct* Entity;
  struct Djinni_Renderable_SpriteStruct* Sprite;
  struct Djinni_Renderable_ShapeStruct*  Shape;
  struct Djinni_Renderable_PaintStruct*  Paint;

  void (*initialize)();
  void (*draw)(Renderer*, Entity*, Camera*);
};

extern struct Djinni_RenderableStruct Djinni_Renderable;

#endif