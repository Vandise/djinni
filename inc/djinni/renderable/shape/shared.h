#ifndef DJINNI_RENDERABLE_SHAPE_SHARED
#define DJINNI_RENDERABLE_SHAPE_SHARED 1

#include "djinni/util/shared.h"
#include "djinni/renderable/shared.h"
#include "djinni/geometry/shared.h"

#include "djinni/video/renderer.h"
#include "djinni/geometry/rectangle.h"

typedef enum {
  SHAPE_RECTANGLE_PTR_TYPE
} SHAPE_TYPE;

typedef struct ShapeStruct {
  SHAPE_TYPE type;

  int fill;
  int outline;

  Color outlineColor;
  Color fillColor;

  union {
    Rectangle* rectptr;
  } geometry;
} Shape;

#endif