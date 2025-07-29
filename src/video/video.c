#include "djinni/video/shared.h"

int djinni_video_initialize(int flags) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return -1;
	}

  IMG_Init(flags);

  return 0;
}