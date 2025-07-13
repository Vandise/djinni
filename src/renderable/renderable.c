#include "djinni/util/util.h"
#include "djinni/renderable/renderable.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Renderable.initialize");

  Djinni_Renderable.Entity = &Djinni_Renderable_Entity;
  Djinni_Renderable.Sprite = &Djinni_Renderable_Sprite;
  Djinni_Renderable.Shape  = &Djinni_Renderable_Shape;
  Djinni_Renderable.Paint  = &Djinni_Renderable_Paint;

  Djinni_Renderable.Shape->initialize();
}

static void draw(Renderer* r, Entity* e) {
  Djinni_Renderable_Paint.entity(r, e);
}

struct Djinni_RenderableStruct Djinni_Renderable = {
  .initialize = initialize,
  .draw = draw
};