#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "djinni/djinni.h"

Entity* player = NULL;
Entity* enemy = NULL;
Entity* enemy2 = NULL;

void enemyLeftViewport(Entity* e, Game* g, double dt) {
  printf("Enemy left the viewport: %p \n", e);
}

void enemyEnteredViewport(Entity* e, Game* g, double dt) {
  printf("Enemy entered the viewport: %p \n", e);
}

void playerUpdate(Entity* entity, Game* game, double dt) {}

void onCreate(Stage* self, Game* game, Stage* previous) {
  Camera* camera = Djinni.Game->Camera->create(
    0,0,
    Djinni.windowSettings.width,
    Djinni.windowSettings.height
  );

  Djinni.Game->setCamera(game, camera);
  Djinni.Game->Camera->inspect(camera);

  player = Djinni.Renderable->Sprite->create(0, 0, "bin/gfx/player.png");
  player->update = playerUpdate;
  player->body.velocity.dx = 1;

  enemy = Djinni.Renderable->Sprite->create(0, 50, "bin/gfx/enemy.png");
  enemy2 = Djinni.Renderable->Sprite->create(850, 50, "bin/gfx/enemy.png");
  enemy->onEnterViewport = enemyEnteredViewport;
  enemy->onExitViewport = enemyLeftViewport;
  enemy2->onEnterViewport = enemyEnteredViewport;
  enemy2->onExitViewport = enemyLeftViewport;

  Djinni.Game->World->addEntity(game->world, player);
  Djinni.Game->World->addEntity(game->world, enemy);
  Djinni.Game->World->addEntity(game->world, enemy2);

  Djinni.Game->Camera->follow(camera, player, 0, 0);

  Djinni.Game->enableInput(game);
}

void prepare(Stage* self, Game* game) {}

void update(Stage* self, Game* game, double dt) {
  //Djinni.Game->Camera->inspect(game->camera);
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


  int startX = 100;
  int startY = 100;

  Entity rect = Djinni.Renderable->Shape->Rectangle->rectangle(
    startX,
    startY,
    10, 10
  );
  Djinni.Renderable->Shape->setOutlineColor(&rect, white);
  Djinni.Renderable->Shape->setFillColor(&rect, white);

  Djinni.Renderable->Paint->entity(Djinni.renderer, &rect, game->camera);

  int predrawX = 500;
  int predrawY = 100;

  rect = Djinni.Renderable->Shape->Rectangle->rectangle(
    predrawX,
    predrawY,
    10, 10
  );
  Djinni.Renderable->Shape->setOutlineColor(&rect, blue);
  Djinni.Renderable->Shape->setFillColor(&rect, blue);
  Djinni.Renderable->Paint->entity(Djinni.renderer, &rect, game->camera);


  Coordinate coords = Djinni.Renderable->Entity->getPosition(player);
  rect = Djinni.Renderable->Shape->Rectangle->rectangle(
    coords.x,
    coords.y,
    player->body.bounds.instance.w, player->body.bounds.instance.h
  );
  Djinni.Renderable->Shape->setOutlineColor(&rect, white);
  Djinni.Renderable->Paint->entity(Djinni.renderer, &rect, game->camera);
}

void onDestroy(Stage* self, Game* game, Stage* next) {}

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

  WorldSettings worldSettings = {
    .width = 2000,
    .height = 2000,
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