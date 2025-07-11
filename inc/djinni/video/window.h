#ifndef DJINNI_VIDEO_WINDOW
#define DJINNI_VIDEO_WINDOW 1

#include "djinni/video/shared.h"

typedef struct WindowStruct {
  int width;
  int height;

  SDL_Window* instance;
} Window;

struct Djinni_Video_WindowStruct {
  Window* (*create)(const char *title, int x, int y, int w, int h, int flags);
  void (*destroy)(Window *window);
};

extern struct Djinni_Video_WindowStruct Djinni_Video_Window;

#endif