#ifndef DJINNI_ENGINE
#define DJINNI_ENGINE 1

#define MAX_KEYBOARD_KEYS 350

#include "util/logger.h"
#include "video/video.h"

struct DjinniStruct {
  struct Djinni_Util_LoggerStruct* Logger;
  struct Djinni_VideoStruct* Video;

  void (*initialize)();
  void (*setFlag)(const char* name, const char* value);
};

extern struct DjinniStruct Djinni;

#endif