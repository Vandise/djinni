#ifndef DJINNI_RENDERABLE_DRAW
#define DJINNI_RENDERABLE_DRAW 1

#include "djinni/renderable/shared.h"
#include "djinni/video/shared.h"
#include "djinni/game/camera.h"

struct Djinni_Renderable_PaintStruct {
  void (*entity)(Renderer*, Entity*, Camera*);
  void (*sprite)(Renderer*, Entity*, int, int, float);
  void (*shape)(Renderer*, Shape*, int, int, float);
};

extern struct Djinni_Renderable_PaintStruct Djinni_Renderable_Paint;

#endif