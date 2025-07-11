#ifndef DJINNI_VIDEO
#define DJINNI_VIDEO 1

#include "djinni/video/shared.h"
#include "djinni/video/window.h"
#include "djinni/video/renderer.h"
#include "djinni/video/texture.h"

struct Djinni_VideoStruct {
  struct Djinni_Video_WindowStruct* Window;
  struct Djinni_Video_RendererStruct* Renderer;
  struct Djinni_TextureStruct* Texture;

  int (*initialize)();
};

extern struct Djinni_VideoStruct Djinni_Video;

#endif