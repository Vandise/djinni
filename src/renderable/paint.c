#include <SDL2/SDL.h>
#include "djinni/util/util.h"
#include "djinni/renderable/renderable.h"
#include "djinni/video/video.h"

static void rectangle(Renderer* r, Shape* shape, int x, int y) {
  Rectangle proxy = *(shape->geometry.rectptr);
  proxy.instance.x = x;
  proxy.instance.y = y;

  if (shape->outline) {
    Djinni_Video.Renderer->setDrawColor(r, shape->outlineColor);
    SDL_RenderDrawRect(r->instance, &(proxy.instance));
  }

  if (shape->fill) {
    Djinni_Video.Renderer->setDrawColor(r, shape->fillColor);
    SDL_RenderFillRect(r->instance, &(proxy.instance));
  }

  Djinni_Video.Renderer->resetDrawColor(r);
}

static void shape(Renderer* r, Shape* shape, int x, int y) {
  switch(shape->type) {
    case SHAPE_RECTANGLE_PTR_TYPE:
      rectangle(r, shape, x, y);
      break;
    default:
      break;
  }
}

static void sprite(Renderer* r, Entity* subject, int x, int y) {
  Djinni_Video.Texture->blit(
    r, subject->texture, x, y,
    Djinni_Renderable.Entity->getRenderedWidth(subject),
    Djinni_Renderable.Entity->getRenderedHeight(subject)
  );
}

/*
  todo: handle camera 
*/

static void entity(Renderer* r, Entity* subject, Camera* camera) {
  Coordinate pos = Djinni_Renderable.Entity->getRenderPoint(subject);

  int x = pos.x - camera->point.x;
  int y = pos.y - camera->point.y;

  switch(subject->type) {
    case ENTITY_TYPE_SPRITE:
      sprite(r, subject, x, y);
      break;
    case ENTITY_TYPE_SHAPE:
      shape(r, &(subject->shape), x, y);
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