#include "djinni/ecs/ecs.h"
#include "djinni/game/camera.h"
#include "djinni/video/video.h"
#include "djinni/ecs/systems/collision_system.h"

static int rectangle_overlaps(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) {
  return ax < bx + bw && ax + aw > bx &&
         ay < by + bh && ay + ah > by;
}

static void process_collisions(DjinniEntityId id, double dt) {
  Djinni_Collidable* collision_box = djinni_ecs_component_collision_get(id);
  Djinni_Position* position = djinni_ecs_component_position_get(id);
  Djinni_Behavior* behaviors = djinni_ecs_component_behavior_get(id);
  Djinni_GridLocation grid_cache = collision_box->grid_cache;

  int has_collide = behaviors->collide == NULL ? 0 : 1;

  Djinni_Collidable* other_box;
  Djinni_Position*   other_position;

  for (int i = 0; i < grid_cache.cells->used; i++) {
    Djinni_GridCell* cell = grid_cache.cells->data[i];

    if (cell->entities->used > 0) {
      for (int j = 0; j < cell->entities->used; j++) {
        DjinniEntityId other = *((int*)cell->entities->data[j]);

        //
        // do not process self against self or process both (A,B) and (B,A)
        //
        if (id == other || other < id) { continue; }

        other_box = djinni_ecs_component_collision_get(other);
        other_position = djinni_ecs_component_position_get(other);

        if (rectangle_overlaps(
          position->x, position->y, collision_box->width, collision_box->height,
          other_position->x, other_position->y, other_box->width, other_box->height
        )) {

          if (has_collide) { behaviors->collide(id, other, dt); }

          if (other_box->solid) {
            Djinni_Velocity* velocity = djinni_ecs_component_velocity_get(id);
              position->x -= velocity->x;
              position->y -= velocity->y;
              velocity->x = 0;
              velocity->y = 0;
          }
        }
      }
    }
  }
}

void djinni_ecs_collision_system(DjinniEntityId id, double dt) {
  Djinni_Grid* grid = djinni_grid_state_get_grid();

  //
  // update cell position
  //
  if (djinni_grid_concern_entity_needs_cell_update(grid, id)) {
    djinni_grid_remove(grid, id);
    djinni_grid_insert(grid, id, djinni_grid_concern_compute_ring(id));
  }

  //
  // process cells this entity belongs to
  //
  if (djinni_ecs_component_dirty_is_dirty(id)) {
    process_collisions(id, dt);
  }

  djinni_ecs_component_dirty_clear(id);
}