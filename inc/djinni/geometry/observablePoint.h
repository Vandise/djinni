#ifndef DJINNI_GEOMETRY_OBSERVABLE_POINT
#define DJINNI_GEOMETRY_OBSERVABLE_POINT 1

#include "djinni/geometry/shared.h"
#include "djinni/geometry/coordinate.h"

/*
  As an anchor:
    0,0 = top left
    1,1 = bottom right
*/
typedef struct ObservablePointStruct {
  float x;
  float y;
} ObservablePoint;

struct Djinni_Geometry_ObservablePointStruct {
  Coordinate (*translate)(ObservablePoint, int, int, int, int);
};

extern struct Djinni_Geometry_ObservablePointStruct Djinni_Geometry_ObservablePoint;

#endif