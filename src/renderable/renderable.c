#include "djinni/util/util.h"
#include "djinni/renderable/renderable.h"

void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Renderable.initialize");

  Djinni_Renderable.Entity = &Djinni_Renderable_Entity;
  Djinni_Renderable.Sprite = &Djinni_Renderable_Sprite;
  Djinni_Renderable.Shape  = &Djinni_Renderable_Shape;

  Djinni_Renderable.Shape->initialize();
}

struct Djinni_RenderableStruct Djinni_Renderable = {
  .initialize = initialize
};