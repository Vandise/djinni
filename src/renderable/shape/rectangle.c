#include "djinni/util/util.h"
#include "djinni/renderable/shape/rectangle.h"

static Entity* create(int x, int y, int w, int h);

static Entity rectangle(int x, int y, int w, int h) {
  Entity* rectptr = create(x,y,w,h);
  Entity rect = *rectptr;
  
  Djinni_Renderable.Entity->destroy(rectptr);

  return rect;
}

static Entity* create(int x, int y, int w, int h) {
  Entity* e = Djinni_Renderable.Entity->create(x, y, w, h, ENTITY_TYPE_SHAPE);

  e->shape.type = SHAPE_RECTANGLE_PTR_TYPE;
  e->shape.fill = 0;
  e->shape.outline = 0;
  e->shape.geometry.rectptr = &(e->body.bounds);

  return e;
}

static void inspect(Entity* e) {
  Coordinate loc = Djinni_Renderable.Entity->getRenderedPosition(e);

  Djinni_Util_Logger.log_debug(
    "Djinni::Renderable::Shape::Rectangle(address:(%p), fill:(%d), outline:(%d), x:(%d), y:(%d))",
    e, e->shape.fill, e->shape.outline, loc.x, loc.y
  );
}

struct Djinni_Renderable_Shape_RectangleStruct Djinni_Renderable_Shape_Rectangle = {
  .rectangle = rectangle,
  .create = create,
  .inspect = inspect
};