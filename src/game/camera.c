#include "djinni/util/util.h"
#include "djinni/game/camera.h"

static Camera* create(int x, int y, int rWidth, int rHeight) {
  Camera* c = malloc(sizeof(Camera));
  c->renderWidth = rWidth;
  c->renderHeight = rHeight;
  c->point.x = x;
  c->point.y = y;

  return c;
}

static int inViewport(Camera* c, Point p) {
  int x1, x2, y1, y2;
  int rw = c->renderWidth;
  int rh = c->renderHeight;

  x1 = c->point.x + ((rw/2) * -1);
  x2 = x1 + rw;

  y1 = c->point.y + ((rh/2) * -1);
  y2 = y1 + rh;

  int x = p.x;
  int y = p.y;

  if (x < x1 || x > x2 || y < y1 || y > y2) {
    return 0;
  }

  return 1;
}

static void inspect(Camera* c) {
  int x1, x2, y1, y2;
  int rw = c->renderWidth;
  int rh = c->renderHeight;

  x1 = c->point.x + ((rw/2) * -1);
  x2 = x1 + rw;

  y1 = c->point.y + ((rh/2) * -1);
  y2 = y1 + rh;

  Djinni_Util_Logger.log_debug(
    "Djinni::Game::Camera( address:(%p) w:(%d) h:(%d) Point:( x:(%d) y:(%d) ) Bounds:( x1:(%d) x2:(%d) y1:(%d) y2:(%d) ) )",
    c, rw, rh, c->point.x, c->point.y, x1, x2, y1, y2
  );
}

static void destroy(Camera* c) {
  free(c);
}

struct Djinni_Game_GameCameraStruct Djinni_Camera = {
  .create = create,
  .inViewport = inViewport,
  .inspect = inspect,
  .destroy = destroy
};