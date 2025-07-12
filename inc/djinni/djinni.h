#ifndef DJINNI_ENGINE
#define DJINNI_ENGINE 1

#define MAX_KEYBOARD_KEYS 350

#include <SDL2/SDL.h>
#include "djinni/util/util.h"
#include "djinni/video/video.h"
#include "djinni/geometry/geometry.h"
#include "djinni/renderable/renderable.h"

typedef struct WindowSettingsStruct {
  char* name;

  int posx;
  int posy;
  int width;
  int height;

  int flags;
} WindowSettings;

typedef struct VideoSettingsStruct {
  int index;
  int rendererFlags;
  int videoFlags;
} VideoSettings;

struct DjinniStruct {
  Window *window;
  Renderer *renderer;

  struct WindowSettingsStruct windowSettings;
  struct VideoSettingsStruct videoSettings;

  struct Djinni_Util_LoggerStruct* Logger;
  struct Djinni_VideoStruct* Video;
  struct Djinni_GeometryStruct* Geometry;
  struct Djinni_RenderableStruct* Renderable;

  int (*initialize)(WindowSettings, VideoSettings);
  void (*setFlag)(const char* name, const char* value);
  void (*terminate)();
};

extern struct DjinniStruct Djinni;

#endif