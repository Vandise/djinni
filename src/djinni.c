#include "djinni.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni.initialize");

  Djinni_Video.initialize();

  Djinni.Logger = &Djinni_Util_Logger;
  Djinni.Video = &Djinni_Video;
}

static void setFlag(const char *name, const char *value) {
  SDL_SetHint(name, value);
}

struct DjinniStruct Djinni = {
  .initialize = initialize,
  .setFlag = setFlag
};