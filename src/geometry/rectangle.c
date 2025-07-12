#include "djinni/util/util.h"
#include "djinni/geometry/rectangle.h"
#include "djinni/geometry/coordinate.h"

static Rectangle create(int x, int y, int w, int h) {
  Rectangle r;
    r.instance.x = x;
    r.instance.y = y;
    r.instance.w = w;
    r.instance.h = h;

  return r;
}

static Coordinate getPosition(Rectangle* r) {
  return Djinni_Geometry_Coordinate.create(r->instance.x, r->instance.y);
}

static void setPosition(Rectangle* r, int x, int y) {
  r->instance.x = x;
  r->instance.y = y;
}

static void inspect(Rectangle* r) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Geometry::Rectangle( address:(%p) x:(%d) y:(%d) w:(%d) h:(%d) )",
    r, r->instance.x, r->instance.y, r->instance.w, r->instance.h
  );
}

struct Djinni_Geometry_RectangleStruct Djinni_Geometry_Rectangle = {
  .create = create,
  .getPosition = getPosition,
  .setPosition = setPosition,
  .inspect = inspect
};