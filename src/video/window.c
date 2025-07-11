#include "video/window.h"

static Window* create(const char *title, int x, int y, int w, int h, int flags) {
  Window *winstance = malloc(sizeof(Window));
  winstance->width = w;
  winstance->height = h;

  winstance->instance = SDL_CreateWindow(title, x, y, w, h, flags);

  return winstance;
}

static void destroy(Window *window) {
  SDL_DestroyWindow(window->instance);
  free(window);
}

struct Djinni_Video_WindowStruct Djinni_Video_Window = {
  .create = create,
  .destroy = destroy
};