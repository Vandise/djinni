#ifndef DJINNI
#define DJINNI 1

#include "djinni/common.h"
#include "djinni/debug/debug.h"
#include "djinni/ecs/ecs.h"
#include "djinni/video/video.h"
#include "djinni/game/game.h"
#include "djinni/game/camera.h"
#include "djinni/render/render.h"
#include "djinni/grid/grid.h"
#include "djinni/map/map.h"

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
void djinni_start(int stage_id);
void djinni_destroy();

#endif