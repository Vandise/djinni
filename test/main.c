#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "djinni/djinni.h"

int terminate = 0;

void pollEvents() {
  SDL_Event event;
    while (SDL_PollEvent(&event)) {
      printf("Pull Event \n");
        switch (event.type) {
            case SDL_QUIT:
                terminate = 1;
                break;

            default:
                break;
        }
    }

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
    .index = -1,
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

  Entity* e = Djinni.Renderable->Entity->create(0,0,10,10, ENTITY_TYPE_NONE);
  Djinni.Renderable->Entity->inspect(e);
  Djinni.Renderable->Entity->destroy(e);


  while (terminate == 0) {

    printf("Draw \n");

    Djinni.Video->Renderer->drawColor(Djinni.renderer, 96, 128, 255, 255);

    printf("Clear \n");

    Djinni.Video->Renderer->clear(Djinni.renderer);

    pollEvents();
    
    printf("Present \n");

    Djinni.Video->Renderer->present(Djinni.renderer);
    
    SDL_Delay(1000);
  }

  printf("Terminate \n");

  Djinni.terminate();

  return 0;
}