#ifndef DJINNI_GEOMETRY_SHARED
#define DJINNI_GEOMETRY_SHARED 1

#include "geometry/shared.h"
#include "geometry/coordinate.h"
#include "geometry/line.h"

struct Djinni_GeometryStruct {
  struct Djinni_Geometry_CoordinateStruct* Coordinate;
  struct Djinni_Geometry_LineStruct* Line;

  void (*initialize)();
};

extern struct Djinni_GeometryStruct Djinni_Geometry;

#endif