#include "djinni/util/util.h"
#include "djinni/game/camera.h"

static Camera* create(int x, int y, int screenWidth, int screenHeight) {
  Camera* c = malloc(sizeof(Camera));
  c->screenWidth = screenWidth;
  c->screenHeight = screenHeight;
  c->zoom = 1.0;
  c->point.x = x;
  c->point.y = y;
  c->settings.fixed = 1;
  c->settings.xOffset = 0;
  c->settings.yOffset = 0;
  c->settings.following = NULL;

  c->update = NULL;

  return c;
}

static void setDrawPoint(Camera* c, Point p) {
  c->point.x = p.x;
  c->point.y = p.y;
}

static Point coordinateToScreen(Camera* c, Coordinate coords) {
  if (c->settings.fixed) {
    return coords;
  }

  Point p = {
    .x = coords.x - c->point.x,
    .y = coords.y - c->point.y
  };

  return p;
}

//
// converts a given Entity Coordinate X-Y to a anchored position point
//
static Point entityCoordinateToScreen(Camera* c, Entity* entity) {
  Coordinate coords = Djinni_Renderable_Entity.getPosition(entity);
  return coordinateToScreen(c, coords);
}

static ViewportBounds getViewportBounds(Camera* c) {
  int x1, x2, y1, y2;
  int rw = c->screenWidth;
  int rh = c->screenHeight;

  x1 = c->point.x;
  x2 = x1 + rw;

  y1 = c->point.y;
  y2 = y1 + rh;

  ViewportBounds bounds = {
    .x1 = x1,
    .x2 = x2,
    .y1 = y1,
    .y2 = y2
  };;

  return bounds;
}

/*

todo: review camera incorporatation of point to position
      when follow is set camera point x/y are the x1 + y1
*/
static int inViewport(Camera* c, Point p) {
  int x1, x2, y1, y2;
  int rw = c->screenWidth;
  int rh = c->screenHeight;

  x1 = c->point.x;
  x2 = x1 + rw;

  y1 = c->point.y;
  y2 = y1 + rh;

  int x = p.x;
  int y = p.y;

  if (x < x1 || x > x2 || y < y1 || y > y2) {
    return 0;
  }

  return 1;
}

static void update(Camera* c, double dt) {
  if (!c->settings.fixed && c->settings.following != NULL) {
    Coordinate pos = Djinni_Renderable_Entity.getPosition(c->settings.following);
    c->point.x = (pos.x - (c->screenWidth / 2)) + c->settings.xOffset;
    c->point.y = (pos.y - (c->screenHeight / 2)) + c->settings.yOffset;
  }
}

static void follow(Camera* c, Entity* e, int xOffset, int yOffset) {
  c->settings.fixed = 0;
  c->settings.xOffset = xOffset;
  c->settings.yOffset = yOffset;
  c->settings.following = e;

  Coordinate coords = Djinni_Renderable_Entity.getPosition(c->settings.following);

  c->point.x = (coords.x - (c->screenWidth / 2)) + c->settings.xOffset;
  c->point.y = (coords.y - (c->screenHeight / 2)) + c->settings.yOffset;
}

static void inspect(Camera* c) {
  int x1, x2, y1, y2;
  int rw = c->screenWidth;
  int rh = c->screenHeight;

  x1 = c->point.x;
  x2 = x1 + rw;

  y1 = c->point.y;
  y2 = y1 + rh;

  Djinni_Util_Logger.log_debug(
    "Djinni::Game::Camera( address:(%p) w:(%d) h:(%d) Point:( x:(%d) y:(%d) ) Bounds:( x1:(%d) x2:(%d) y1:(%d) y2:(%d) ) Following:( address: %p ) )",
    c, rw, rh, c->point.x, c->point.y, x1, x2, y1, y2, c->settings.following
  );
}

static void destroy(Camera* c) {
  free(c);
}

struct Djinni_Game_GameCameraStruct Djinni_Camera = {
  .create = create,
  .setDrawPoint = setDrawPoint,
  .inViewport = inViewport,
  .entityCoordinateToScreen = entityCoordinateToScreen,
  .coordinateToScreen = coordinateToScreen,
  .getViewportBounds = getViewportBounds,
  .follow = follow,
  .update = update,
  .inspect = inspect,
  .destroy = destroy
};