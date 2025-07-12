#ifndef DJINNI_VIDEO_TEXTURE
#define DJINNI_VIDEO_TEXTURE 1

#include "djinni/video/shared.h"
#include "djinni/geometry/rectangle.h"

typedef struct TextureStruct {
  SDL_Texture *instance;
  Rectangle bounds;
} Texture;

struct Djinni_TextureStruct {
  void (*initialize)(int flags);
  Texture* (*load)(Renderer*, char*);
  void (*blit)(Renderer*, Texture*, int, int);
  void (*destroy)(Texture*);
};

extern struct Djinni_TextureStruct Djinni_Video_Texture;

#endif