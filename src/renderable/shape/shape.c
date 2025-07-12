#include "djinni/util/util.h"
#include "djinni/renderable/shape/shape.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Renderable::Shape.initialize");

  Djinni_Renderable_Shape.Rectangle = &Djinni_Renderable_Shape_Rectangle;
}

static void setOutlineColor(Entity* e, Color c) {
  e->shape.outline = 1;
  e->shape.outlineColor = c;
}

static void setFillColor(Entity* e, Color c) {
  e->shape.fill = 1;
  e->shape.fillColor = c;
}

struct Djinni_Renderable_ShapeStruct Djinni_Renderable_Shape = {
  .initialize = initialize,
  .setOutlineColor = setOutlineColor,
  .setFillColor = setFillColor
};