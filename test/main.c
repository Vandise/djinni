#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "djinni/djinni.h"

int terminate = 0;

void pollEvents() {
  SDL_Event* event = malloc(sizeof(SDL_Event));
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                terminate = 1;
                break;

            default:
                break;
        }
    }
  free(event);
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


  Coordinate c = Djinni.Geometry->Coordinate->create(10,10);
  Coordinate c2 = Djinni.Geometry->Coordinate->create(20,20);

  Djinni.Geometry->Coordinate->inspect(&c);

  Line l = Djinni.Geometry->Line->create(c, c2);
  Djinni.Geometry->Line->inspect(&l);

  Rectangle r = Djinni.Geometry->Rectangle->create(0,0,10,20);
  Djinni.Geometry->Rectangle->inspect(&r);

  Entity* e = Djinni.Renderable->Sprite->create(100,100,"bin/gfx/player.png");

  while (terminate == 0) {
    Djinni.Video->Renderer->drawColor(Djinni.renderer, 0, 0, 0, 255);
    Djinni.Video->Renderer->clear(Djinni.renderer);

    pollEvents();

    Djinni.Renderable->Entity->move(e,5,0);
    Coordinate position = Djinni.Renderable->Entity->getPosition(e);

    Djinni.Video->Texture->blit(
      Djinni.renderer,
      e->texture,
      position.x, position.y
    );

    Djinni.Video->Renderer->present(Djinni.renderer);

    Djinni.Logger->log_debug("Terminate( %d )", terminate);

    SDL_Delay(1000);
  }

  Djinni.Renderable->Entity->destroy(e);

  Djinni.terminate();

  return 0;
}