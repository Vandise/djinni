#include "djinni/video/renderer.h"
#include "djinni/video/window.h"

static SDL_Renderer* renderer;
static Color background_color;

void djinni_video_renderer_initialize(int index, int flags) {
  renderer = SDL_CreateRenderer(djinni_video_window_get(), index, flags);

  background_color.r = 0;
  background_color.g = 0;
  background_color.b = 0;
  background_color.a = 255;
}

void djinni_video_renderer_reset_draw_color() {
  SDL_SetRenderDrawColor(
    renderer,
    background_color.r,
    background_color.g,
    background_color.b,
    background_color.a
  );
}

void djinni_video_renderer_set_background_color(Color c) {
  background_color = c;
}

void djinni_video_renderer_set_draw_color(Color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

void djinni_video_renderer_present() {
  SDL_RenderPresent(renderer);
}

void djinni_video_renderer_clear() {
  SDL_RenderClear(renderer);
}

void djinni_video_renderer_destroy() {
  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);
  }
}