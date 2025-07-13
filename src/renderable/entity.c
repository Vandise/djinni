#include "djinni/util/util.h"
#include "djinni/geometry/geometry.h"
#include "djinni/renderable/entity.h"

static Coordinate getPosition(Entity* e);

static Entity* create(int x, int y, int w, int h, ENTITY_TYPE type) {
  Entity* e = malloc(sizeof(Entity));
  e->type = type;

  e->anchorPoint.x = ANCHOR_DEFAULT;
  e->anchorPoint.y = ANCHOR_DEFAULT;

  Coordinate pos = Djinni_Geometry.ObservablePoint->translate(
    e->anchorPoint, x, y, w, h
  );

  e->bounds = Djinni_Geometry.Rectangle->create(pos.x, pos.y, w, h);
  e->body = Djinni_Physics.Body->create(pos.x, pos.y, w, h);

  e->status = ENTITY_ALIVE;
  e->alwaysUpdate = 0;
  e->keepAlive = 0;
  e->texture = NULL;
  e->children = NULL;

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

static Coordinate getRenderedPosition(Entity* e) {
  return Djinni_Geometry.Rectangle->getPosition(&(e->body.bounds));
}

static void move(Entity* e, int dx, int dy) {
  Coordinate c = Djinni_Geometry.Rectangle->getPosition(&(e->body.bounds));

  setPosition(e, c.x + dx, c.y + dy);
}

static void setAnchor(Entity* e, float x, float y) {
  Coordinate pt = getPosition(e);

  e->anchorPoint.x = x;
  e->anchorPoint.y = y;

  Coordinate pos = Djinni_Geometry.ObservablePoint->translate(
    e->anchorPoint,
    pt.x, pt.y,
    e->body.bounds.instance.w, e->body.bounds.instance.h
  );

  setPosition(e, pos.x, pos.y);
}

static int getRenderedWidth(Entity* e) {
  return e->bounds.instance.w;
}

static int getRenderedHeight(Entity* e) {
  return e->bounds.instance.h;
}

static int getBodyWidth(Entity* e) {
  return e->body.bounds.instance.w;
}

static int getBodyHeight(Entity* e) {
  return e->body.bounds.instance.h;
}

static Coordinate getPosition(Entity* e) {
  Coordinate rendPos = getRenderedPosition(e);

  Coordinate pos = Djinni_Geometry.ObservablePoint->getAnchorPoint(
    e->anchorPoint, rendPos.x, rendPos.y, getRenderedWidth(e), getRenderedHeight(e)
  );

  return pos;
}

static void scale(Entity* e, float ws, float hs) {
  // todo: Shape entities other than Rect need to be scaled
  //       Rescale children

  Djinni_Geometry.Rectangle->resize(
    &(e->bounds),
    getRenderedWidth(e) * ws,
    getRenderedHeight(e) * hs
  );

  Djinni_Geometry.Rectangle->resize(
    &(e->body.bounds),
    getBodyWidth(e) * ws,
    getBodyHeight(e) * hs
  );
}

static void inspect(Entity* e) {
  Coordinate pos = getPosition(e);

  Djinni_Util_Logger.log_debug(
    "Djinni::Renderable::Entity( address:(%p) type:(%d) status:(%d) alwaysUpdate:(%d) keepAlive:(%d) position:(x:%d y:%d))",
    e, e->type, e->status, e->alwaysUpdate, e->keepAlive, pos.x, pos.y
  );
  Djinni_Geometry.Rectangle->inspect(&(e->bounds));
  Djinni_Physics.Body->inspect(&(e->body));

  Coordinate c = getRenderedPosition(e);
  Djinni_Geometry.Coordinate->inspect(&c);
}

static void destroy(Entity* e) {
  free(e);
}

struct Djinni_Renderable_EntityStruct Djinni_Renderable_Entity = {
  .create = create,
  .getPosition = getPosition,
  .getRenderedPosition = getRenderedPosition,
  .inspect = inspect,
  .move = move,
  .setPosition = setPosition,
  .setAnchor = setAnchor,
  .getRenderedWidth = getRenderedWidth,
  .getRenderedHeight = getRenderedHeight,
  .getBodyWidth = getBodyWidth,
  .getBodyHeight = getBodyHeight,
  .scale = scale,
  .destroy = destroy
};