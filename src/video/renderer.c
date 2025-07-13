#include "djinni/util/util.h"
#include "djinni/video/window.h"
#include "djinni/video/renderer.h"

static Renderer* create(Window *window, int index, int flags) {
  Djinni_Util_Logger.log_dev("Djinni::Video::Renderer.create");

  Renderer *r = malloc(sizeof(Renderer));
  r->instance = SDL_CreateRenderer(window->instance, index, flags);

  r->backgroundColor.r = 0;
  r->backgroundColor.g = 0;
  r->backgroundColor.b = 0;
  r->backgroundColor.a = 255;

  return r;
}

static void resetDrawColor(Renderer *renderer) {
  Color c = renderer->backgroundColor;

  SDL_SetRenderDrawColor(renderer->instance, c.r, c.g, c.b, c.a);
}

static void setBackgroundColor(Renderer *renderer, Color c) {
  renderer->backgroundColor = c;
}

static void setDrawColor(Renderer *renderer, Color c) {
  SDL_SetRenderDrawColor(renderer->instance, c.r, c.g, c.b, c.a);
}

static void present(Renderer *renderer) {
  SDL_RenderPresent(renderer->instance);
}

static void clear(Renderer *renderer) {
  SDL_RenderClear(renderer->instance);
}

static void destroy(Renderer *renderer) {
  Djinni_Util_Logger.log_dev("Djinni::Video::Renderer.destroy (%p)", renderer);

  if (renderer != NULL && renderer->instance != NULL) {
    SDL_DestroyRenderer(renderer->instance);
    free(renderer);
  }
}

struct Djinni_Video_RendererStruct Djinni_Video_Renderer = {
  .create = create,
  .destroy = destroy,
  .setBackgroundColor = setBackgroundColor,
  .setDrawColor = setDrawColor,
  .resetDrawColor = resetDrawColor,
  .present = present,
  .clear = clear
};