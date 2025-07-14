#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "djinni/djinni.h"

Entity* player = NULL;
Entity* enemy = NULL;

void playerUpdate(Entity* entity, Game* game, double dt) {
  Djinni.Renderable->Entity->move(player, 1, 0);
}

void onCreate(Stage* self, Game* game, Stage* previous) {
  player = Djinni.Renderable->Sprite->create(0, 0, "bin/gfx/player.png");
  player->update = playerUpdate;

  enemy = Djinni.Renderable->Sprite->create(0, 50, "bin/gfx/enemy.png");

  Djinni.Game->World->addEntity(game->world, player);
  Djinni.Game->World->addEntity(game->world, enemy);

  Camera* camera = Djinni.Game->Camera->create(
    0,0,
    Djinni.windowSettings.width,
    Djinni.windowSettings.height
  );

  Djinni.Game->Camera->follow(camera, player, 0, 0);

  Djinni.Game->setCamera(game, camera);
  Djinni.Game->Camera->inspect(camera);

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
    startX - game->camera->point.x,
    startY - game->camera->point.y,
    10, 10
  );
  Djinni.Renderable->Shape->setOutlineColor(&rect, white);
  Djinni.Renderable->Shape->setFillColor(&rect, white);
  Djinni.Renderable->draw(Djinni.renderer, &rect, game->camera);

  int predrawX = 500;
  int predrawY = 300;

  rect = Djinni.Renderable->Shape->Rectangle->rectangle(
    predrawX - game->camera->point.x,
    predrawY - game->camera->point.y,
    10, 10
  );
  Djinni.Renderable->Shape->setOutlineColor(&rect, blue);
  Djinni.Renderable->Shape->setFillColor(&rect, blue);
  Djinni.Renderable->draw(Djinni.renderer, &rect, game->camera);


  Point p = Djinni.Game->Camera->entityCoordinateToScreen(game->camera, player);
  rect = Djinni.Renderable->Shape->Rectangle->rectangle(
    p.x,
    p.y,
    player->body.bounds.instance.w, player->body.bounds.instance.h
  );
  Djinni.Renderable->Shape->setOutlineColor(&rect, white);
  Djinni.Renderable->draw(Djinni.renderer, &rect, game->camera);
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

  Game* game = Djinni.Game->create();
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