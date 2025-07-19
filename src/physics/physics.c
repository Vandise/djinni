#include "djinni/util/util.h"
#include "djinni/physics/physics.h"
#include "djinni/renderable/renderable.h"
#include "djinni/game/game.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Physics.initialize");

  Djinni_Physics.Body = &Djinni_Physics_Body;
  Djinni_Physics.Collision = &Djinni_Physics_Collision;
}

static int logDT = 0;
static void tick(Game* game, double dt, DJINNI_RING ring) {
  GridLevel* level = &game->world->grid->levels[ring];

  for (int y = 0; y < level->height; y++) {
    for (int x = 0; x < level->width; x++) {
      
      GridCell* cell = &level->cells[y * level->width + x];

      if (cell->entities->used > 0) {
        for (int i = 0; i < cell->entities->used; i++) {
          Entity* entity = cell->entities->data[i];

          Djinni_Physics_Body.tickVelocity(entity, dt);

          //
          // only check for collisions if the entity has moved
          //
          if (entity->dirty) {
            Djinni_Physics_Collision.tick(
              game,
              entity,
              cell,
              i,
              dt
            );
          }

        }
      }

    }
  }
}

struct Djinni_PhysicsStruct Djinni_Physics = {
  .initialize = initialize,
  .tick = tick
};