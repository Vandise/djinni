#include "djinni/ecs/ecs.h"
#include "djinni/game/camera.h"
#include "djinni/grid/grid.h"

DJINNI_GRID_RING djinni_grid_concern_compute_ring(DjinniEntityId id) {
  Djinni_Position* position = djinni_ecs_component_position_get(id);
  Djinni_Game_Viewport viewport = djinni_game_camera_get_viewport();

  if (
    position->x > viewport.x1 && position->x < viewport.x2 &&
    position->y > viewport.y1 && position->y < viewport.y2
  ) {
    return DJINNI_RING_FINE;
  }

  int screen_width = viewport.x2 - viewport.x1;
  int screen_height = viewport.y2 - viewport.y1;

  int ptx = viewport.x1 - screen_width / 2;
  int pty = viewport.y1 - screen_height / 2;

  if (
    position->x > ptx && position->x < (screen_width * 2) &&
    position->y > pty && position->y < (screen_height * 2)
  ) {
    return DJINNI_RING_MEDIUM;
  }

  return DJINNI_RING_COARSE;
}

int djinni_grid_concern_entity_needs_cell_update(Djinni_Grid* grid, DjinniEntityId id) {
  Djinni_Position* position = djinni_ecs_component_position_get(id);
  Djinni_Collidable* collision_box = djinni_ecs_component_collision_get(id);

  int result = 0;

  if (collision_box->grid_cache.level > -1) {
    int cell_size = grid->levels[collision_box->grid_cache.level].cell_size;
    int rendered_width = collision_box->width;
    int rendered_height = collision_box->height;

    int minX = (int)((position->x + collision_box->parent_x_offset) / cell_size);
    int minY = (int)((position->y + collision_box->parent_y_offset) / cell_size);
    int maxX = (int)((position->x + collision_box->parent_x_offset + rendered_width) / cell_size);
    int maxY = (int)((position->y + collision_box->parent_y_offset + rendered_height) / cell_size);

    if (minX != collision_box->grid_cache.minX || minY != collision_box->grid_cache.minY ||
        maxX != collision_box->grid_cache.maxX || maxY != collision_box->grid_cache.maxY
    ) {
      result = 1;
    }
  }

  return result;
}