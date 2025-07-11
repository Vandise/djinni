#include "djinni/video/texture.h"
#include "djinni/video/renderer.h"

static void initialize(int flags) {
  IMG_Init(flags);
}

static Texture* load(Renderer *r, char *filename) {
  Texture *t = malloc(sizeof(Texture));

  t->instance = IMG_LoadTexture(r->instance, filename);

  return t;
}

static void blit(Renderer *r, Texture *texture, int x, int y) {
  SDL_Rect dest;

  dest.x = x;
  dest.y = y;

  SDL_QueryTexture(texture->instance, NULL, NULL, &dest.w, &dest.h);
  SDL_RenderCopy(r->instance, texture->instance, NULL, &dest);
}

static void destroy(Texture *t) {
  SDL_DestroyTexture(t->instance);
  free(t);
}

struct Djinni_TextureStruct Djinni_Video_Texture = {
  .initialize = initialize,
  .load = load,
  .blit = blit,
  .destroy = destroy
};