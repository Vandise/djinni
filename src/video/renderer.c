#include "djinni/video/window.h"
#include "djinni/video/renderer.h"

static Renderer* create(Window *window, int index, int flags) {
  Renderer *r = malloc(sizeof(Renderer));
  r->instance = SDL_CreateRenderer(window->instance, index, flags);

  return r;
}

static void drawColor(Renderer *renderer, int r, int g, int b, int a) {
  SDL_SetRenderDrawColor(renderer->instance, r, g, b, a);
}

static void present(Renderer *renderer) {
  SDL_RenderPresent(renderer->instance);
}

static void clear(Renderer *renderer) {
  SDL_RenderClear(renderer->instance);
}

static void destroy(Renderer *renderer) {
  SDL_DestroyRenderer(renderer->instance);
  free(renderer);
}

struct Djinni_Video_RendererStruct Djinni_Video_Renderer = {
  .create = create,
  .destroy = destroy,
  .drawColor = drawColor,
  .present = present,
  .clear = clear
};