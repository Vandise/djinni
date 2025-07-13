#ifndef DJINNI_GEOMETRY_RECTANGLE
#define DJINNI_GEOMETRY_RECTANGLE 1

#include <SDL2/SDL_rect.h>
#include "djinni/geometry/shared.h"

typedef struct RectangleStruct {
  SDL_Rect instance;
} Rectangle;

struct Djinni_Geometry_RectangleStruct {
  Rectangle (*create)(int, int, int, int);
  Coordinate (*getPosition)(Rectangle*);
  void (*setPosition)(Rectangle*, int, int);
  void (*resize)(Rectangle*, int, int);
  void (*inspect)(Rectangle*);
};

extern struct Djinni_Geometry_RectangleStruct Djinni_Geometry_Rectangle;

#endif