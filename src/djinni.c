#include "djinni/djinni.h"
#include "djinni/util/util.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni.initialize");

  Djinni_Video.initialize();
  Djinni_Geometry.initialize();

  Djinni.Logger = &Djinni_Util_Logger;
  Djinni.Video = &Djinni_Video;
  Djinni.Geometry = &Djinni_Geometry;
}

static void setFlag(const char *name, const char *value) {
  SDL_SetHint(name, value);
}

struct DjinniStruct Djinni = {
  .initialize = initialize,
  .setFlag = setFlag
};