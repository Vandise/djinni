#ifndef DJINNI_GEOMETRY_LINE
#define DJINNI_GEOMETRY_LINE 1

#include "djinni/geometry/shared.h"
#include "djinni/geometry/coordinate.h"

typedef struct LineSlopeStruct {
  float dx;
  float dy;
} LineSlope;

typedef struct LineStruct {
  Coordinate start;
  Coordinate end;

  LineSlope slope;
} Line;

struct Djinni_Geometry_LineStruct {
  Line (*create)(Coordinate, Coordinate);
  LineSlope (*slope)(Coordinate, Coordinate);
  void (*inspect)(Line*);
};

extern struct Djinni_Geometry_LineStruct Djinni_Geometry_Line;

#endif