#ifndef DJINNI_VIDEO
#define DJINNI_VIDEO 1

#include "djinni/video/shared.h"
#include "djinni/video/window.h"
#include "djinni/video/renderer.h"
#include "djinni/video/texture.h"
#include "djinni/video/imageAtlas.h"

struct Djinni_VideoStruct {
  Renderer* renderer;

  struct Djinni_Video_WindowStruct* Window;
  struct Djinni_Video_RendererStruct* Renderer;
  struct Djinni_TextureStruct* Texture;
  struct Djinni_Video_ImageAtlasStruct* ImageAtlas;

  int (*initialize)(int);
  void (*setRenderer)(Renderer*);
};

extern struct Djinni_VideoStruct Djinni_Video;

#endif