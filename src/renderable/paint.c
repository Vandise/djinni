#include <SDL2/SDL.h>
#include "djinni/util/util.h"
#include "djinni/renderable/renderable.h"
#include "djinni/video/video.h"

static void rectangle(Renderer* r, Shape* shape, int x, int y) {
  if (shape->outline) {
    Djinni_Video.Renderer->setDrawColor(r, shape->outlineColor);
    SDL_RenderDrawRect(r->instance, &(shape->geometry.rectptr->instance));
  }

  if (shape->fill) {
    //Djinni_Video.Renderer->setDrawColor(r, rectColor);
    //SDL_RenderFillRect(SDL_Renderer * renderer, const SDL_Rect * rect);
  }
}

static void shape(Renderer* r, Entity* subject, int x, int y) {
  switch(subject->shape.type) {
    case SHAPE_RECTANGLE_PTR_TYPE:
      rectangle(r, &(subject->shape), x, y);
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

static void entity(Renderer* r, Entity* subject) {
  Coordinate pos = Djinni_Renderable.Entity->getRenderedPosition(subject);

  switch(subject->type) {
    case ENTITY_TYPE_SPRITE:
      sprite(r, subject, pos.x, pos.y);
      break;
    case ENTITY_TYPE_SHAPE:
      shape(r, subject, pos.x, pos.y);
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