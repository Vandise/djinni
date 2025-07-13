#include "djinni/util/util.h"
#include "djinni/renderable/renderable.h"
#include "djinni/video/video.h"

static void shape(Renderer* r, Entity* subject, int x, int y) {
  
}

static void sprite(Renderer* r, Entity* subject, int x, int y) {
  Djinni_Video.Texture->blit(
    r, subject->texture, x, y,
    Djinni_Renderable.Entity->getRenderedWidth(subject),
    Djinni_Renderable.Entity->getRenderedHeight(subject)
  );
}

static void entity(Renderer* r, Entity* subject) {
  Coordinate pos = Djinni_Renderable.Entity->getRenderedPosition(subject);

  switch(subject->type) {
    case ENTITY_TYPE_SPRITE:
      sprite(r, subject, pos.x, pos.y);
      break;
    default:
      break;
  }
}

struct Djinni_Renderable_PaintStruct Djinni_Renderable_Paint = {
  .entity = entity,
  .sprite = sprite,
  .shape = shape
};