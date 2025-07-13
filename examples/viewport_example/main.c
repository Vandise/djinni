#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "djinni/djinni.h"

Entity* player = NULL;

void playerUpdate(Entity* entity, Game* game, double dt) {
  Point loc = Djinni.Renderable->Entity->getPosition(entity);
  if( Djinni.Game->Camera->inViewport(game->camera, loc) ) {
    Djinni.Renderable->Entity->move(entity, -1, 0);
  }
}

void onCreate(Stage* self, Game* game, Stage* previous) {
  player = Djinni.Renderable->Sprite->create(100,100,"bin/gfx/player.png");
  player->update = playerUpdate;

  Djinni.Game->World->addEntity(game->world, player);

  Camera* camera = Djinni.Game->Camera->create(
    Djinni.windowSettings.width/2,
    Djinni.windowSettings.height/2,
    Djinni.windowSettings.width,
    Djinni.windowSettings.height
  );

  Djinni.Game->setCamera(game, camera);

  Djinni.Game->Camera->inspect(camera);

  Djinni.Game->enableInput(game);
}

void prepare(Stage* self, Game* game) {}

void update(Stage* self, Game* game, double dt) {}

void draw(Stage* self, Game* game, double dt) {
  Color white = {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = 255
  };


  Entity rect = Djinni.Renderable->Shape->Rectangle->rectangle(100,100, 10, 10);
  Djinni.Renderable->Shape->setOutlineColor(&rect, white);
  Djinni.Renderable->Shape->setFillColor(&rect, white);
  Djinni.Renderable->draw(Djinni.renderer, &rect);

  Coordinate pos = Djinni.Renderable->Entity->getRenderedPosition(player);

  Shape playerRect;
    playerRect.type = SHAPE_RECTANGLE_PTR_TYPE;
    playerRect.outline = 1;
    playerRect.fill = 0;
    playerRect.outlineColor = white;
    playerRect.fillColor = white;
    playerRect.geometry.rectptr = &(player->body.bounds);
  Djinni.Renderable->Paint->shape(Djinni.renderer, &playerRect, pos.x, pos.y);
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