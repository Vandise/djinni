#ifndef DJINNI_VIDEO_RENDERER
#define DJINNI_VIDEO_RENDERER 1

#include "video/shared.h"

typedef struct RendererStruct {
  SDL_Renderer* instance;
} Renderer;

struct Djinni_Video_RendererStruct {
  Renderer* (*create)(Window *window, int index, int flags);
  void (*destroy)(Renderer *renderer);

  void (*drawColor)(Renderer *renderer, int r, int g, int b, int a);
  void (*present)(Renderer *renderer);
  void (*clear)(Renderer *renderer);
};

extern const struct Djinni_Video_RendererStruct Djinni_Video_Renderer;

#endif