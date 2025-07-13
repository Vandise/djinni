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

  Color background = {
    .r = 0,
    .g = 0,
    .b = 0,
    .a = 255
  };

  Color rectColor = {
    .r = 0,
    .g = 0,
    .b = 255,
    .a = 255
  };

  Color yellow = {
    .r = 255,
    .g = 255,
    .b = 0,
    .a = 255
  };

  Color ptColor = {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = 255
  };

  Djinni.Video->Renderer->setBackgroundColor(Djinni.renderer, background);

  Entity* e = Djinni.Renderable->Sprite->create(100,100,"bin/gfx/player.png");
  //Djinni.Renderable->Entity->inspect(e);

  int i = 0;
  float ax[] = {0, 0.5, 1, 0, 1};
  float ay[] = {0, 0.5, 0, 1, 1};

  while (terminate == 0) {
    if (i+1>5) { i = 0; }

    Djinni.Video->Renderer->setDrawColor(Djinni.renderer, Djinni.renderer->backgroundColor);
    Djinni.Video->Renderer->clear(Djinni.renderer);

    pollEvents();

    //Djinni.Renderable->Entity->move(e,5,0);
    Djinni.Renderable->Entity->setAnchor(e,ax[i],ay[i]);

    Coordinate position = Djinni.Renderable->Entity->getRenderedPosition(e);
    Coordinate anchorPos = Djinni.Renderable->Entity->getPosition(e);

/*
    Djinni.Video->Texture->blit(
      Djinni.renderer,
      e->texture,
      position.x, position.y,
      Djinni.Renderable->Entity->getRenderedWidth(e),
      Djinni.Renderable->Entity->getRenderedHeight(e)
    );
*/

    Djinni.Video->Renderer->setDrawColor(Djinni.renderer, rectColor);
    SDL_RenderDrawRect(Djinni.renderer->instance, &(e->bounds.instance));

    SDL_RenderDrawPoint(Djinni.renderer->instance,anchorPos.x,anchorPos.y);
    SDL_RenderDrawPoint(Djinni.renderer->instance,anchorPos.x,anchorPos.y-1);
    SDL_RenderDrawPoint(Djinni.renderer->instance,anchorPos.x-1,anchorPos.y);
    SDL_RenderDrawPoint(Djinni.renderer->instance,anchorPos.x-1,anchorPos.y-1);

    Djinni.Video->Renderer->setDrawColor(Djinni.renderer, yellow);
    SDL_RenderDrawPoint(Djinni.renderer->instance, 100, 100);
    SDL_RenderDrawPoint(Djinni.renderer->instance, 99, 99);
    SDL_RenderDrawPoint(Djinni.renderer->instance, 98, 98);
    SDL_RenderDrawPoint(Djinni.renderer->instance, 100, 99);
    SDL_RenderDrawPoint(Djinni.renderer->instance, 99, 100);
  
  
    Djinni.Video->Renderer->present(Djinni.renderer);

    //Djinni.Renderable->Entity->scale(e, 1.1, 1.1);

    SDL_Delay(2000);
    i++;
  }

  Djinni.Renderable->Entity->destroy(e);

  Djinni.terminate();

  return 0;
}