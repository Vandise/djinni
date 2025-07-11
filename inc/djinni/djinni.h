#ifndef DJINNI_ENGINE
#define DJINNI_ENGINE 1

#define MAX_KEYBOARD_KEYS 350

#include "djinni/util/shared.h"
#include "djinni/video/video.h"
#include "djinni/geometry/geometry.h"

struct DjinniStruct {
  struct Djinni_Util_LoggerStruct* Logger;
  struct Djinni_VideoStruct* Video;
  struct Djinni_GeometryStruct* Geometry;

  void (*initialize)();
  void (*setFlag)(const char* name, const char* value);
};

extern struct DjinniStruct Djinni;

#endif