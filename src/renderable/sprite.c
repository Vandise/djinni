#include "djinni/util/util.h"
#include "djinni/renderable/sprite.h"
#include "djinni/video/video.h"

static Entity* create(int x, int y, char* filename) {
  Texture* t = Djinni_Video.Texture->load(Djinni_Video.renderer, filename);  

  Entity* e = Djinni_Renderable_Entity.create(
    x, y, t->bounds.instance.w, t->bounds.instance.h, ENTITY_TYPE_SPRITE
  );

  e->texture = t;

  return e;
}

struct Djinni_Renderable_SpriteStruct Djinni_Renderable_Sprite = {
  .create = create
};