#include "camerafocus.h"

static double switchMs = 0;
static int cameraPosIndex = 0;
static Point points[8];

void onCreate(Stage* self, Game* game, Stage* previous) {
  Camera* camera = Djinni.Game->Camera->create(
    0,0,
    Djinni.windowSettings.width,
    Djinni.windowSettings.height
  );

  camera->update = NULL;
  Djinni.Game->setCamera(game, camera);
  Djinni.Game->Camera->inspect(camera);

  Color white = {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = 255
  };


  ViewportBounds bounds = Djinni.Game->Camera->getViewportBounds(camera);
  int boundsX[] = { bounds.x1, bounds.x2, ((bounds.x1 + bounds.x2)/2) };
  int boundsY[] = { bounds.y1, bounds.y2, ((bounds.y1 + bounds.y2)/2) };


  int i = 0;
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      Point p = {
        .x = boundsX[x],
        .y = boundsY[y]
      };

      points[i++] = p;

      Entity* rect = Djinni.Renderable->Shape->Rectangle->create(
        p.x,
        p.y,
        20, 20
      );

      Djinni.Renderable->Shape->setOutlineColor(rect, white);
      Djinni.Renderable->Shape->setFillColor(rect, white);

      Djinni.Game->World->addEntity(game->world, rect);
    }
  }

  Djinni.Game->enableInput(game);
}

void prepare(Stage* self, Game* game) {}

void update(Stage* self, Game* game, double dt) {
  if ((dt - switchMs) > 2000) {
    switchMs = dt;

    Point drawPoint = points[cameraPosIndex];
    Djinni.Game->Camera->setDrawPoint(game->camera, drawPoint);
    Djinni.Game->Camera->inspect(game->camera);

    if (cameraPosIndex++ >= 7) {
      cameraPosIndex = 0;
    }
  }
}

void draw(Stage* self, Game* game, double dt) {
  Color white = {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = 255
  };

  Color blue = {
    .r = 0,
    .g = 0,
    .b = 255,
    .a = 255
  };
}

void onDestroy(Stage* self, Game* game, Stage* next) {}