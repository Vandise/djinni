#include <SDL2/SDL.h>
#include "djinni/util/util.h"
#include "djinni/renderable/renderable.h"
#include "djinni/video/video.h"

static void rectangle(Renderer* r, Shape* shape, int x, int y, float zoom) {
  Rectangle proxy = *(shape->geometry.rectptr);
  proxy.instance.x = x * zoom;
  proxy.instance.y = y * zoom;
  proxy.instance.w = proxy.instance.w * zoom;
  proxy.instance.h = proxy.instance.h * zoom;

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

static void shape(Renderer* r, Shape* shape, int x, int y, float zoom) {
  switch(shape->type) {
    case SHAPE_RECTANGLE_PTR_TYPE:
      rectangle(r, shape, x, y, zoom);
      break;
    default:
      break;
  }
}

static void sprite(Renderer* r, Entity* subject, int x, int y, float zoom) {
  Djinni_Video.Texture->blit(
    r, subject->texture, x * zoom, y * zoom,
    Djinni_Renderable.Entity->getRenderedWidth(subject) * zoom,
    Djinni_Renderable.Entity->getRenderedHeight(subject) * zoom
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
      sprite(r, subject, x, y, camera->zoom);
      break;
    case ENTITY_TYPE_SHAPE:
      shape(r, &(subject->shape), x, y, camera->zoom);
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