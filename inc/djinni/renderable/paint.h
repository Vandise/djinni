#ifndef DJINNI_RENDERABLE_DRAW
#define DJINNI_RENDERABLE_DRAW 1

#include "djinni/renderable/shared.h"
#include "djinni/video/shared.h"

struct Djinni_Renderable_PaintStruct {
  void (*entity)(Renderer*, Entity*);
  void (*sprite)(Renderer*, Entity*, int, int);
  void (*shape)(Renderer*, Shape*, int, int);
};

extern struct Djinni_Renderable_PaintStruct Djinni_Renderable_Paint;

#endif