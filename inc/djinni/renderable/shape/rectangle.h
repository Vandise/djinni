#ifndef DJINNI_RENDERABLE_SHAPE_RECTANGLE
#define DJINNI_RENDERABLE_SHAPE_RECTANGLE 1

#include "djinni/renderable/shape/shared.h"
#include "djinni/renderable/renderable.h"

struct Djinni_Renderable_Shape_RectangleStruct {
  Entity (*rectangle)(int,int,int,int);
  Entity* (*create)(int,int,int,int);
  void (*inspect)(Entity*);
};

extern struct Djinni_Renderable_Shape_RectangleStruct Djinni_Renderable_Shape_Rectangle;

#endif