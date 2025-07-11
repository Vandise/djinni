#include "djinni/util/util.h"
#include "djinni/renderable/sprite.h"
#include "djinni/video/texture.h"

static Entity* create(int x, int y, const char* filename) {
  //Djinni_Renderable_Entity
  return NULL;
}

struct Djinni_Renderable_SpriteStruct Djinni_Renderable_Sprite = {
  .create = create
};