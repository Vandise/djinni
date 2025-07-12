#ifndef DJINNI_VIDEO_RENDERER
#define DJINNI_VIDEO_RENDERER 1

#include "djinni/video/shared.h"

typedef struct ColorStruct {
  int r;
  int g;
  int b;
  int a;
} Color;

typedef struct RendererStruct {
  SDL_Renderer* instance;
  Color backgroundColor;
} Renderer;

struct Djinni_Video_RendererStruct {
  Renderer* (*create)(Window *window, int index, int flags);
  void (*destroy)(Renderer *renderer);

  void (*setBackgroundColor)(Renderer *renderer, Color c);
  void (*setDrawColor)(Renderer *renderer, Color c);
  void (*present)(Renderer *renderer);
  void (*clear)(Renderer *renderer);
};

extern struct Djinni_Video_RendererStruct Djinni_Video_Renderer;

#endif