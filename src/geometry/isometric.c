#include "djinni/geometry/isometric.h"
#include "djinni/geometry/coordinate.h"
#include <stdio.h>

/*
mt->sx = ((mt->x * mapLayer->tiles.tileWidth / 2) + (mt->y * mapLayer->tiles.tileWidth / 2));

mt->sy = (m->height / 2) - ((mt->y * (mapLayer->tiles.tileHeight / 2) / 2) - (mt->x * (mapLayer->tiles.tileHeight / 2) / 2));
*/
static Coordinate translate(int mh, int x, int y, int w, int h) {
  int cx = ((x * w / 2) + (y * w / 2));
  int cy = (mh / 2) - ((y * (h / 2) / 2) - (x * (h / 2) / 2));

  Coordinate c = {
    .x = cx,
    .y = cy
  };

  return c;
}

static Coordinate cartesianToISO(int mh, int mw, int tw, int th, int x, int y) {
  Coordinate c = translate(
    mh,
    (x % ((mw / tw))),
    (y / ((mh / th))),
    tw,
    th
  );
  return c;
}

struct Djinni_Geometry_IsometricStruct Djinni_Geometry_Isometric = {
  .translate = translate,
  .cartesianToISO = cartesianToISO
};