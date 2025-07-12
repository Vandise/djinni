#ifndef DJINNI_RENDERABLE_SHAPE
#define DJINNI_RENDERABLE_SHAPE 1

#include "djinni/renderable/shape/shared.h"
#include "djinni/renderable/shape/rectangle.h"

struct Djinni_Renderable_ShapeStruct {
  struct Djinni_Renderable_Shape_RectangleStruct* Rectangle;

  void (*initialize)();
  void (*setOutlineColor)(Entity*,Color);
  void (*setFillColor)(Entity*,Color);
};

extern struct Djinni_Renderable_ShapeStruct Djinni_Renderable_Shape;

#endif