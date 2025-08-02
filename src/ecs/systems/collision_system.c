#include "djinni/ecs/ecs.h"
#include "djinni/game/camera.h"
#include "djinni/video/video.h"
#include "djinni/ecs/systems/collision_system.h"

void djinni_ecs_collision_system(DJINNI_GRID_RING ring) {
  Djinni_Grid* grid = djinni_grid_state_get_grid();
  Djinni_GridLevel* level = &(grid->levels[ring]);

  for (int y = 0; y < level->height; y++) {
    for (int x = 0; x < level->width; x++) {
      Djinni_GridCell* cell = &level->cells[y * level->width + x];

      if (cell->entities->used > 0) {
        for (int i = 0; i < cell->entities->used; i++) {

          DjinniEntityId id = *((int*)cell->entities->data[i]);

          if (djinni_ecs_component_includes(id, DJINNI_COMPONENT_COLLIDABLE)) {
            //
            // update cell position
            //
            if (djinni_grid_concern_entity_needs_cell_update(grid, id)) {
              djinni_grid_remove(grid, id);
              djinni_grid_insert(grid, id, djinni_grid_concern_compute_ring(id));
            }

            //
            // todo: handle collision check
            //
            if (djinni_ecs_component_dirty_is_dirty(id)) {
              djinni_ecs_component_dirty_clear(id);
            }
          }
        }
      }
    }
  }
}