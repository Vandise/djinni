#include "djinni/video/window.h"

static SDL_Window* window = NULL;

void djinni_video_window_initialize(const char *title, int x, int y, int w, int h, int flags) {
  window = SDL_CreateWindow(title, x, y, w, h, flags);
}

SDL_Window* djinni_video_window_get() {
  return window;
}

void djinni_video_window_destroy() {
  if (window != NULL) {
    SDL_DestroyWindow(window);
  }
}