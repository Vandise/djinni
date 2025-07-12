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

  Color ptColor = {
    .r = 255,
    .g = 255,
    .b = 0,
    .a = 255
  };

  Djinni.Video->Renderer->setBackgroundColor(Djinni.renderer, background);

  Coordinate c = Djinni.Geometry->Coordinate->create(10,10);
  Coordinate c2 = Djinni.Geometry->Coordinate->create(20,20);

  Djinni.Geometry->Coordinate->inspect(&c);

  Line l = Djinni.Geometry->Line->create(c, c2);
  Djinni.Geometry->Line->inspect(&l);

  Rectangle r = Djinni.Geometry->Rectangle->create(0,0,10,20);
  Djinni.Geometry->Rectangle->inspect(&r);

  Entity* e = Djinni.Renderable->Sprite->create(100,100,"bin/gfx/player.png");
  //Djinni.Renderable->Entity->setAnchor(e,0,0);
  Djinni.Renderable->Entity->inspect(e);

  Entity* rctr = Djinni.Renderable->Shape->Rectangle->create(100,100,10,10);

  Entity* rtl = Djinni.Renderable->Shape->Rectangle->create(100,100,10,10);
  Djinni.Renderable->Entity->setAnchor(rtl,0,0);

  Entity* rtr = Djinni.Renderable->Shape->Rectangle->create(100,100,10,10);
  Djinni.Renderable->Entity->setAnchor(rtr,1,0);

  Entity* rbl = Djinni.Renderable->Shape->Rectangle->create(100,100,10,10);
  Djinni.Renderable->Entity->setAnchor(rbl,0,1);

  Entity* rbr = Djinni.Renderable->Shape->Rectangle->create(100,100,10,10);
  Djinni.Renderable->Entity->setAnchor(rbr,1,1);

  while (terminate == 0) {
    Djinni.Video->Renderer->setDrawColor(Djinni.renderer, Djinni.renderer->backgroundColor);
    Djinni.Video->Renderer->clear(Djinni.renderer);

    pollEvents();

    //Djinni.Renderable->Entity->move(e,5,0);
    Coordinate position = Djinni.Renderable->Entity->getPosition(e);
/*
    Djinni.Video->Texture->blit(
      Djinni.renderer,
      e->texture,
      position.x, position.y
    );
*/
    Djinni.Video->Renderer->setDrawColor(Djinni.renderer, rectColor);
  
    //SDL_RenderDrawRect(Djinni.renderer->instance, &(rectEntity->bounds.instance));
    SDL_RenderDrawRect(Djinni.renderer->instance, &(e->bounds.instance));
    SDL_RenderDrawRect(Djinni.renderer->instance, &(rtr->bounds.instance));
    SDL_RenderDrawRect(Djinni.renderer->instance, &(rtl->bounds.instance));
    SDL_RenderDrawRect(Djinni.renderer->instance, &(rbr->bounds.instance));
    SDL_RenderDrawRect(Djinni.renderer->instance, &(rbl->bounds.instance));
    SDL_RenderDrawRect(Djinni.renderer->instance, &(rctr->bounds.instance));

    Djinni.Video->Renderer->setDrawColor(Djinni.renderer, ptColor);
    SDL_RenderDrawPoint(Djinni.renderer->instance,100,100);
    SDL_RenderDrawPoint(Djinni.renderer->instance,100,99);
    SDL_RenderDrawPoint(Djinni.renderer->instance,99,100);
    SDL_RenderDrawPoint(Djinni.renderer->instance,99,99);
  
    Djinni.Video->Renderer->present(Djinni.renderer);

    Djinni.Logger->log_debug("Terminate( %d )", terminate);

    SDL_Delay(1000);
  }

  Djinni.Renderable->Entity->destroy(e);

  Djinni.terminate();

  return 0;
}