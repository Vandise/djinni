#include <stdio.h>
#include <stdlib.h>
#include "djinni.h"

int main(void) {
  Djinni.initialize();
  Djinni.Video->Texture->initialize(IMG_INIT_PNG | IMG_INIT_JPG);
  Djinni.setFlag(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  return 0;
}