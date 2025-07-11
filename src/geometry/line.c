#include "util/util.h"
#include "geometry/line.h"

static LineSlope slope(Coordinate c1, Coordinate c2);

static Line create(Coordinate c1, Coordinate c2) {
  Line line;
  line.start = c1;
  line.end = c2;
  line.slope = slope(line.start, line.end);

  return line;
}

static LineSlope slope(Coordinate c1, Coordinate c2) {
  LineSlope slope;

  int steps = MAX(abs(c1.x - c2.x), abs(c1.y - c2.y));

  if (steps == 0) {
    slope.dx = slope.dy = 0;
    return slope;
  }

  slope.dx = (c1.x - c2.x);
  slope.dx /= steps;

  slope.dy = (c1.y - c2.y);
  slope.dy /= steps;

  return slope;
}

static void inspect(Line* l) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Geometry::Line(\n\taddress:(%p) \n\tx1:(%d) y1:(%d) \n\tx2:(%d) y2:(%d) \n\tdx:(%f) dy:(%f)\n)",
    l, l->start.x, l->start.y, l->end.x, l->end.y, l->slope.dx, l->slope.dy
  );
}

struct Djinni_Geometry_LineStruct Djinni_Geometry_Line = {
  .create = create,
  .slope = slope,
  .inspect = inspect
};