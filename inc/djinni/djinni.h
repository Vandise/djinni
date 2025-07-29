#ifndef DJINNI
#define DJINNI 1

#include "djinni/video/video.h"
#include "djinni/game/game.h"

typedef struct Djinni_WindowSettingsStruct {
  char* name;

  int posx;
  int posy;
  int width;
  int height;

  int flags;
} Djinni_WindowSettings;

typedef struct Djinni_VideoSettingsStruct {
  int index;
  int rendererFlags;
  int videoFlags;
} Djinni_VideoSettings;

void djinni_initialize(Djinni_WindowSettings ws, Djinni_VideoSettings vs, Djinni_GameSettings gs);
void djinni_set_flag(const char *name, const char *value);
void djinni_destroy();

#endif