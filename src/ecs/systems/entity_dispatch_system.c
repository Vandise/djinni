#include "djinni/ecs/ecs.h"
#include "djinni/ecs/systems/entity_dispatch_system.h"

void djinni_ecs_system_dispatch(DJINNI_GRID_RING ring, double dt) {
  Djinni_Grid* grid = djinni_grid_state_get_grid();
  Djinni_GridLevel* level = &(grid->levels[ring]);

  for (int y = 0; y < level->height; y++) {
    for (int x = 0; x < level->width; x++) {
      Djinni_GridCell* cell = &level->cells[y * level->width + x];

      if (cell->entities->used > 0) {
        for (int i = 0; i < cell->entities->used; i++) {

          //
          // todo: tag as static and skip all updating
          //
          DjinniEntityId id = *((int*)cell->entities->data[i]);
          Djinni_Behavior* behaviors = djinni_ecs_component_behavior_get(id);

          //
          // avoid processing the entity on the same frame
          //
          if (behaviors->last_tick == dt) { continue; }

          if (behaviors->update != NULL) { behaviors->update(id, dt); }

          if (djinni_ecs_component_includes(id, DJINNI_COMPONENT_VELOCITY)) {
            djinni_ecs_system_velocity(id, dt);
          }

          if (djinni_ecs_component_includes(id, DJINNI_COMPONENT_COLLIDABLE)) {
            djinni_ecs_collision_system(id, dt);
          }

          behaviors->last_tick = dt;
        }
      }
    }
  }
}