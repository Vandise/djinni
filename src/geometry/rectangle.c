#include "util/util.h"
#include "geometry/rectangle.h"

static Rectangle create(int x, int y, int w, int h) {
  Rectangle r;
    r.instance.x = x;
    r.instance.y = y;
    r.instance.w = w;
    r.instance.h = h;

  return r;
}

static void inspect(Rectangle* r) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Geometry::Rectangle( address:(%p) x:(%d) y:(%d) w:(%d) h:(%d) )",
    r, r->instance.x, r->instance.y, r->instance.w, r->instance.h
  );
}

struct Djinni_Geometry_RectangleStruct Djinni_Geometry_Rectangle = {
  .create = create,
  .inspect = inspect
};