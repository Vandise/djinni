#ifndef DJINNI_GEOMETRY_COORDINATE
#define DJINNI_GEOMETRY_COORDINATE 1

#include "geometry/shared.h"

typedef struct CoordinateStruct {
  int x;
  int y;
} Coordinate;

struct Djinni_Geometry_CoordinateStruct {
  Coordinate (*create)(int, int);

  void (*inspect)(Coordinate*);
};

extern struct Djinni_Geometry_CoordinateStruct Djinni_Geometry_Coordinate;

#endif