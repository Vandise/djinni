#include <stdio.h>
#include <stdlib.h>
#include "djinni/djinni.h"

int main(void) {
  Djinni.initialize();
  Djinni.Video->Texture->initialize(IMG_INIT_PNG | IMG_INIT_JPG);
  Djinni.setFlag(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  Coordinate c = Djinni.Geometry->Coordinate->create(10,10);
  Coordinate c2 = Djinni.Geometry->Coordinate->create(20,20);

  Djinni.Geometry->Coordinate->inspect(&c);

  Line l = Djinni.Geometry->Line->create(c, c2);
  Djinni.Geometry->Line->inspect(&l);

  Rectangle r = Djinni.Geometry->Rectangle->create(0,0,10,20);
  Djinni.Geometry->Rectangle->inspect(&r);

  return 0;
}