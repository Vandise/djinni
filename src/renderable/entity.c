#include "djinni/util/util.h"
#include "djinni/geometry/geometry.h"
#include "djinni/renderable/entity.h"

static Coordinate getPosition(Entity* e);
static Entity* create(int x, int y, int w, int h, ENTITY_TYPE type);
static void destroy(Entity* e);
static void onTerminate(Entity* e);

static Entity entity(int x, int y, int w, int h, ENTITY_TYPE type) {
  Entity* eptr = create(x,y,w,h,type);
  Entity e = *eptr;

  destroy(eptr);

  return e;
}

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
  e->isoRef = NULL;

  e->status = ENTITY_ALIVE;
  e->id = -1;

  e->dirty = 0;
  e->alwaysUpdate = 0;
  e->keepAlive = 0;
  e->deleteFlag = 0;


  e->texture = NULL;
  e->children = NULL;
  e->data = NULL;
  e->worldMapData = NULL;

  for(int i = 0; i < DJINNI_GRID_MAX_LEVELS; i++) {
    e->locations[i].level = -1;
    e->locations[i].minX = 0;
    e->locations[i].minY = 0;
    e->locations[i].maxX = 0;
    e->locations[i].maxY = 0;
    e->locations[i].cells = NULL;
  }

  e->update = NULL;
  e->onCollide = NULL;
  e->onEnterViewport = NULL;
  e->onExitViewport = NULL;
  e->onDestroy = NULL;
  e->onTerminate = onTerminate;

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

static Coordinate getRenderPoint(Entity* e) {
  return Djinni_Geometry.Rectangle->getPosition(&(e->body.bounds));
}

static void move(Entity* e, int dx, int dy) {
  Coordinate c = Djinni_Geometry.Rectangle->getPosition(&(e->body.bounds));
  setPosition(e, c.x + dx, c.y + dy);

  // needs grid update flag
  e->dirty = 1;
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
  Coordinate rendPos = getRenderPoint(e);

  Coordinate pos = Djinni_Geometry.ObservablePoint->getAnchorPoint(
    e->anchorPoint, rendPos.x, rendPos.y, getRenderedWidth(e), getRenderedHeight(e)
  );

  return pos;
}

static void scale(Entity* e, float ws, float hs) {
  // todo: Shape entities other than Rect need to be scaled
  //       Rescale children

  Coordinate pt = getPosition(e);

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

  Coordinate pos = Djinni_Geometry.ObservablePoint->translate(
    e->anchorPoint,
    pt.x, pt.y,
    e->body.bounds.instance.w, e->body.bounds.instance.h
  );

  setPosition(e, pos.x, pos.y);
}

static void inspect(Entity* e) {
  Coordinate pos = getPosition(e);

  Djinni_Util_Logger.log_debug(
    "Djinni::Renderable::Entity( address:(%p) type:(%d) status:(%d) alwaysUpdate:(%d) keepAlive:(%d) position:(x:%d y:%d))",
    e, e->type, e->status, e->alwaysUpdate, e->keepAlive, pos.x, pos.y
  );

  Djinni_Geometry.Rectangle->inspect(&(e->bounds));
  Djinni_Physics.Body->inspect(&(e->body));

  Coordinate c = getRenderPoint(e);
  Djinni_Geometry.Coordinate->inspect(&c);

  for(int i = 0; i < DJINNI_GRID_MAX_LEVELS; i++) {
    GridLocation* location = &e->locations[i];

    Djinni_Util_Logger.log_debug("\tDjinni::Renderable::Entity( level:(%d) )",i);

    if (location->cells != NULL) {
      for (int j = 0; j < location->cells->used; j++) {
        GridCell* cell = location->cells->data[j];
        Djinni_Util_Logger.log_debug(
          "\t\tDjinni::Renderable::Entity( cell:(%p) n-entities:(%d) )",
          cell, cell->entities->used
        );

        for (int k = 0; k < cell->entities->used; k++) {
          Djinni_Util_Logger.log_debug(
            "\t\t\tDjinni::Renderable::Entity( entity:(%p) )",
            cell->entities->data[k]
          );
        }
      }
    }
  }
}

static void destroy(Entity* e) {
  Djinni_Util_Logger.log_dev("Djinni::Renderable::Entity.destroy( address:(%p) )", e);

  for(int i = 0; i < DJINNI_GRID_MAX_LEVELS; i++) {
    GridLocation* location = &e->locations[i];
    if (location->cells != NULL) {
      Djinni_Util_Array.destroy(location->cells, NULL);
      location->cells = NULL;
    }
  }

  if (e->texture != NULL) {
    Djinni_Video.Texture->destroy(e->texture);
  }

  if (e->worldMapData != NULL) {
    free(e->worldMapData);
  }

  free(e);
}

static void onTerminate(Entity* e) {
  if(e->data != NULL) {
    free(e->data);
  }
}

static void arrayDestroyCallback(void* e) {
  Djinni_Util_Logger.log_dev("Djinni::Renderable::Entity.arrayDestroyCallback()");

  onTerminate(e);
  destroy(e);
}

struct Djinni_Renderable_EntityStruct Djinni_Renderable_Entity = {
  .entity = entity,
  .create = create,
  .getPosition = getPosition,
  .getRenderPoint = getRenderPoint,
  .inspect = inspect,
  .move = move,
  .setPosition = setPosition,
  .setAnchor = setAnchor,
  .getRenderedWidth = getRenderedWidth,
  .getRenderedHeight = getRenderedHeight,
  .getBodyWidth = getBodyWidth,
  .getBodyHeight = getBodyHeight,
  .scale = scale,

  .arrayDestroyCallback = arrayDestroyCallback,
  .destroy = destroy
};