#include "djinni/util/util.h"
#include "djinni/video/video.h"

static int initialize(int flags) {
  Djinni_Util_Logger.log_dev("Djinni::Video.initialize");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
  	Djinni_Util_Logger.log_fatal(
  	  "Djinni (%s)", SDL_GetError()
    );
    return -1;
	}

  Djinni_Video.Window = &Djinni_Video_Window;
  Djinni_Video.Renderer = &Djinni_Video_Renderer;
  Djinni_Video.Texture = &Djinni_Video_Texture;
  Djinni_Video.ImageAtlas = &Djinni_Video_Image_Atlas;

  Djinni_Video_Texture.initialize(flags);

  return 0;
}

static void setRenderer(Renderer* renderer) {
  Djinni_Video.renderer = renderer;
}

struct Djinni_VideoStruct Djinni_Video = {
  .initialize = initialize,
  .setRenderer = setRenderer
};