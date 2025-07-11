#include "util/util.h"
#include "geometry/coordinate.h"

static Coordinate create(int x, int y) {
  Coordinate c = {
    .x = x,
    .y = y
  };

  return c;
}

static void inspect(Coordinate* c) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Geometry::Coordinate( address:(%p) x:(%d) y:(%d) )",
    c, c->x, c->y
  );
}

struct Djinni_Geometry_CoordinateStruct Djinni_Geometry_Coordinate = {
  .create = create,
  .inspect = inspect
};