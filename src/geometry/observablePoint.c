#include "djinni/geometry/observablePoint.h"
#include <stdio.h>

static Coordinate getAnchorPoint(ObservablePoint anchor, int x, int y, int w, int h) {
  Coordinate c;

  if (anchor.x < ANCHOR_DEFAULT && anchor.x != 0) {
    c.x = x + w * (ANCHOR_DEFAULT - anchor.x);
  } else {
    c.x = x + w * anchor.x;
  }

  if (anchor.y < ANCHOR_DEFAULT && anchor.y != 0) {
    c.y = y + h * (ANCHOR_DEFAULT - anchor.y);
  } else {
    c.y = y + h * anchor.y;
  }

  return c;
}

static Coordinate translate(ObservablePoint pt, int x, int y, int w, int h) {
  int ax = w * pt.x;
  int ay = h * pt.y;
  int dx = x - ax;
  int dy = y - ay;

  Coordinate c = {
    .x = dx,
    .y = dy
  };

  return c;
}

struct Djinni_Geometry_ObservablePointStruct Djinni_Geometry_ObservablePoint = {
  .translate = translate,
  .getAnchorPoint = getAnchorPoint
};
