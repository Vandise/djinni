#include "djinni/util/util.h"
#include "djinni/video/shared.h"
#include "djinni/video/texture.h"
#include "djinni/video/renderer.h"

static void initialize(int flags) {
  Djinni_Util_Logger.log_dev("Djinni::Video::Texture.initialize");

  IMG_Init(flags);
}

static Texture* load(Renderer *r, char *filename) {
  Djinni_Util_Logger.log_dev("Djinni::Video::Texture.load( name: %s )", filename);

  Texture *t = malloc(sizeof(Texture));
  t->instance = IMG_LoadTexture(r->instance, filename);

  if (t->instance != NULL) {
    SDL_QueryTexture(t->instance, NULL, NULL, &(t->bounds.instance.w), &(t->bounds.instance.h));
  } else {
    Djinni_Util_Logger.log_error("Djinni::Video::Texture.load( name: (%s) status:(failure))", filename);
  }

  return t;
}

static void blit(Renderer *r, Texture *texture, int x, int y, int w, int h) {
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;
  dest.w = h;
  dest.h = h;

  SDL_RenderCopy(r->instance, texture->instance, NULL, &dest);
}

static void destroy(Texture *t) {
  SDL_DestroyTexture(t->instance);
  free(t);
}

static void arrayDestroyCallback(void* t) {
  destroy(t);
}

struct Djinni_TextureStruct Djinni_Video_Texture = {
  .initialize = initialize,
  .load = load,
  .blit = blit,
  .arrayDestroyCallback = arrayDestroyCallback,
  .destroy = destroy
};