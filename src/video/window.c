#include "djinni/util/util.h"
#include "djinni/video/window.h"

static Window* create(const char *title, int x, int y, int w, int h, int flags) {
  Djinni_Util_Logger.log_dev("Djinni::Video::Window.create");

  Window *winstance = malloc(sizeof(Window));
  winstance->width = w;
  winstance->height = h;

  winstance->instance = SDL_CreateWindow(title, x, y, w, h, flags);

  return winstance;
}

static void destroy(Window *window) {
  Djinni_Util_Logger.log_dev("Djinni::Video::Window.destroy (%p)", window);

  if (window != NULL && window->instance != NULL) {
    SDL_DestroyWindow(window->instance);
    free(window);
  }
}

struct Djinni_Video_WindowStruct Djinni_Video_Window = {
  .create = create,
  .destroy = destroy
};