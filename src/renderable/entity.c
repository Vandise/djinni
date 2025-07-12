#include "djinni/util/util.h"
#include "djinni/geometry/geometry.h"
#include "djinni/renderable/entity.h"

static Entity* create(int x, int y, int w, int h, ENTITY_TYPE type) {
  Entity* e = malloc(sizeof(Entity));
  e->type = type;
  e->bounds = Djinni_Geometry.Rectangle->create(x, y, w, h);
  e->body = Djinni_Physics.Body->create(x, y, w, h);
  e->status = ENTITY_ALIVE;
  e->alwaysUpdate = 0;
  e->keepAlive = 0;
  e->texture = NULL;

  return e;
}

static void setPosition(Entity* e, int x, int y) {
  // physic body
  Djinni_Geometry.Rectangle->setPosition(
    &(e->body.bounds), x, y
  );

  // draw bounds
  Djinni_Geometry.Rectangle->setPosition(
    &(e->bounds), x, y
  );
}

static Coordinate getPosition(Entity* e) {
  return Djinni_Geometry.Rectangle->getPosition(&(e->body.bounds));
}

static void move(Entity* e, int dx, int dy) {
  Coordinate c = Djinni_Geometry.Rectangle->getPosition(&(e->body.bounds));

  setPosition(e, c.x + dx, c.y + dy);
}

static void inspect(Entity* e) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Renderable::Entity( address:(%p) type:(%d) status:(%d) alwaysUpdate:(%d) keepAlive:(%d))",
    e, e->type, e->status, e->alwaysUpdate, e->keepAlive
  );
  Djinni_Geometry.Rectangle->inspect(&(e->bounds));
  Djinni_Physics.Body->inspect(&(e->body));

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
  .move = move,
  .setPosition = setPosition,
  .destroy = destroy
};