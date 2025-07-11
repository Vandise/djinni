#include "util/util.h"
#include "video/video.h"

static int initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Video.initialize");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
  	Djinni_Util_Logger.log_fatal(
  	  "Djinni::Video (%s)", SDL_GetError()
    );
    return 1;
	}

  Djinni_Video.Window = &Djinni_Video_Window;
  Djinni_Video.Renderer = &Djinni_Video_Renderer;
  Djinni_Video.Texture = &Djinni_Video_Texture;

  return 0;
}

struct Djinni_VideoStruct Djinni_Video = {
  .initialize = initialize
};