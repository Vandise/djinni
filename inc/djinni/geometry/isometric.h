#ifndef DJINNI_GEOMETRY_ISOMETRIC
#define DJINNI_GEOMETRY_ISOMETRIC 1

#include "djinni/geometry/shared.h"

struct Djinni_Geometry_IsometricStruct {
  Coordinate (*translate)(int, int, int, int, int);
  Coordinate (*cartesianToISO)(int, int, int, int, int, int);
};

extern struct Djinni_Geometry_IsometricStruct Djinni_Geometry_Isometric;

#endif