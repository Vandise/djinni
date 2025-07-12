#include "djinni/util/util.h"
#include "djinni/geometry/geometry.h"
#include "djinni/renderable/entity.h"

static Entity* create(int x, int y, int w, int h, ENTITY_TYPE type) {
  Entity* e = malloc(sizeof(e));
  e->type = type;
  e->bounds = Djinni_Geometry.Rectangle->create(x, y, w, h);
  e->body = Djinni_Geometry.Rectangle->create(x, y, w, h);
  e->status = ENTITY_ALIVE;
  e->alwaysUpdate = 0;
  e->keepAlive = 0;
  e->texture = NULL;

  return e;
}

static Coordinate getPosition(Entity* e) {
  return Djinni_Geometry.Rectangle->getPosition(&(e->bounds));
}

static void inspect(Entity* e) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Renderable::Entity( address:(%p) type:(%d) status:(%d) alwaysUpdate:(%d) keepAlive:(%d))",
    e, e->type, e->status, e->alwaysUpdate, e->keepAlive
  );
  Djinni_Geometry.Rectangle->inspect(&(e->bounds));
  Djinni_Geometry.Rectangle->inspect(&(e->body));

  Coordinate c = getPosition(e);
  Djinni_Geometry.Coordinate->inspect(&c);
}

static void destroy(Entity* e) {
  free(e);
}

struct Djinni_Renderable_EntityStruct Djinni_Renderable_Entity = {
  .create = create,
  .getPosition = getPosition,
  .inspect = inspect,
  .destroy = destroy
};