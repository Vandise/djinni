#include "gridexample.h"

static Entity* player = NULL;
static Grid* grid;

void onCreate(Stage* self, Game* game, Stage* previous) {
  grid = Djinni.Geometry->Grid->create(
    10,
    Djinni.windowSettings.width,
    Djinni.windowSettings.height,
    32,
    64,
    128
  );

  player = Djinni.Renderable->Sprite->create(
    Djinni.windowSettings.width/2,
    Djinni.windowSettings.height/2,
    "bin/gfx/player.png"
  );

  Djinni.Game->World->addEntity(game->world, player);

  Djinni.Game->enableInput(game);

  Djinni.Geometry->Grid->destroy(grid);
}

void prepare(Stage* self, Game* game) {}
void update(Stage* self, Game* game, double dt) {}
void draw(Stage* self, Game* game, double dt) {}
void onDestroy(Stage* self, Game* game, Stage* next) {}