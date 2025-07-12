#ifndef DJINNI_GEOMETRY
#define DJINNI_GEOMETRY 1

#include "djinni/geometry/shared.h"
#include "djinni/geometry/coordinate.h"
#include "djinni/geometry/line.h"
#include "djinni/geometry/rectangle.h"

struct Djinni_GeometryStruct {
  struct Djinni_Geometry_CoordinateStruct* Coordinate;
  struct Djinni_Geometry_LineStruct* Line;
  struct Djinni_Geometry_RectangleStruct* Rectangle;

  void (*initialize)();
};

extern struct Djinni_GeometryStruct Djinni_Geometry;

#endif