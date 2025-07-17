#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "djinni/djinni.h"

int i = 0;
float ax[] = {0.5, 0, 1, 1, 0};
float ay[] = {0.5, 0, 0, 1, 1};
Entity* player = NULL;

void onCreate(Stage* self, Game* game, Stage* previous) {
  Djinni.Logger->log_debug("Stage.onCreate( address:(%p) id:(%d) )", self, self->id);

  player = Djinni.Renderable->Sprite->create(100,100,"bin/gfx/player.png");

  Djinni.Game->World->addEntity(game->world, player);
  //Djinni.Game->World->removeEntity(game->world, e);

  Djinni.Game->enableInput(game);
}

void prepare(Stage* self, Game* game) {}

void update(Stage* self, Game* game, double dt) {
  if (i+1>5) { i = 0; }

  Djinni.Renderable->Entity->setAnchor(player, ax[i], ay[i]);

  i++;
}

void draw(Stage* self, Game* game, double dt) {
  Color white = {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = 255
  };

  Color green = {
    .r = 0,
    .g = 255,
    .b = 0,
    .a = 255
  };

  Entity rect = Djinni.Renderable->Shape->Rectangle->rectangle(100,100, 10, 10);
  Djinni.Renderable->Shape->setOutlineColor(&rect, white);
  Djinni.Renderable->Shape->setFillColor(&rect, white);
  Djinni.Renderable->draw(Djinni.renderer, &rect, game->camera);

  Coordinate pos = Djinni.Renderable->Entity->getRenderPoint(player);


  Shape playerRect;
    playerRect.type = SHAPE_RECTANGLE_PTR_TYPE;
    playerRect.outline = 1;
    playerRect.fill = 0;
    playerRect.outlineColor = green;
    playerRect.geometry.rectptr = &(player->body.bounds);
  Djinni.Renderable->Paint->shape(Djinni.renderer, &playerRect, pos.x, pos.y);
}

void onDestroy(Stage* self, Game* game, Stage* next) {
  Djinni.Logger->log_debug("Stage.onDestroy( address:(%p) id:(%d) )", self, self->id);
}

int main(void) {
  WindowSettings ws = {
    .posx = SDL_WINDOWPOS_UNDEFINED,
    .posy = SDL_WINDOWPOS_UNDEFINED,
    .width = 800,
    .height = 800,
    .flags = 0
  };
  ws.name = strdup("Demo");

  VideoSettings vs = {
    .index = 0,
    .videoFlags = IMG_INIT_PNG | IMG_INIT_JPG,
    .rendererFlags = SDL_RENDERER_ACCELERATED
  };

  Djinni.setFlag(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  Djinni.initialize(ws, vs);

  Djinni.Logger->log_level = DJINNI_LOG_DEBUG;

  Color background = {
    .r = 0,
    .g = 0,
    .b = 0,
    .a = 255
  };

  Djinni.Video->Renderer->setBackgroundColor(Djinni.renderer, background);

  int width;
  int height;

  int finestGridSize;
  int mediumGridSize;
  int coarseGridSize;
  int gridCellCapacity;

  WorldSettings worldSettings = {
    .width = 800,
    .height = 800,
    .finestGridSize = 64,
    .mediumGridSize = 128,
    .coarseGridSize = 256,
    .gridCellCapacity = 10
  };

  Game* game = Djinni.Game->create(worldSettings);
  Stage* s = Djinni.Game->Stage->create(
    0,
    onCreate, prepare, update, draw, onDestroy
  );

  Djinni.Game->addStage(game, s);
  Djinni.Game->changeStage(game, 0);
  Djinni.start(game);

  Djinni.Logger->log_debug("FPS:(%d)", game->stats.fps);

  Djinni.Game->destroy(game);
  Djinni.terminate();

  return 0;
}