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

static void draw(Renderer* r, Grid* grid, Camera* c, DJINNI_RING ring) {
  if (grid == NULL) { return; }

  GridLevel* level = &grid->levels[ring];

  for (int y = 0; y < level->height; y++) {
    for (int x = 0; x < level->width; x++) {
      
      GridCell* cell = &level->cells[y * level->width + x];

      if (cell->entities->used > 0) {
        for (int i = 0; i < cell->entities->used; i++) {
          Djinni_Renderable_Paint.entity(r, cell->entities->data[i], c);
        }
      }

    }
  }
}

struct Djinni_RenderableStruct Djinni_Renderable = {
  .initialize = initialize,
  .draw = draw
};