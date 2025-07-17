#include "djinni/util/util.h"
#include "djinni/physics/physics.h"
#include "djinni/renderable/renderable.h"
#include "djinni/game/game.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Physics.initialize");

  Djinni_Physics.Body = &Djinni_Physics_Body;
}

static void tick(Game* game, double dt, DJINNI_RING ring) {
  GridLevel* level = &game->world->grid->levels[ring];

  for (int y = 0; y < level->height; y++) {
    for (int x = 0; x < level->width; x++) {
      
      GridCell* cell = &level->cells[y * level->width + x];

      if (cell->entities->used > 0) {
        for (int i = 0; i < cell->entities->used; i++) {
          Djinni_Physics_Body.tickVelocity(cell->entities->data[i], dt);
        }
      }

    }
  }
}

struct Djinni_PhysicsStruct Djinni_Physics = {
  .initialize = initialize,
  .tick = tick
};