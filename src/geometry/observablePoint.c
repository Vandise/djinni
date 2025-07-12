#include "djinni/geometry/observablePoint.h"
#include <stdio.h>

static Coordinate translate(ObservablePoint pt, int x, int y, int w, int h) {
  int ax = w * pt.x;
  int ay = h * pt.y;
  int dx = x - ax;
  int dy = y - ay;

  if (pt.x != 0.5) {
    int ox = ((w * 0.5) + x);
    dx = (pt.x < 0.5) ? ox + ax : ox - ax;
  }

  if (pt.y != 0.5) {
    int oy = ((h * 0.5) + y);
    dy = (pt.y < 0.5) ? oy + ay : oy - ay;
  }

  Coordinate c = {
    .x = dx,
    .y = dy
  };

  return c;
}

struct Djinni_Geometry_ObservablePointStruct Djinni_Geometry_ObservablePoint = {
  .translate = translate
};
