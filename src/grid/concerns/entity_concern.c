#include "djinni/ecs/ecs.h"
#include "djinni/game/camera.h"
#include "djinni/grid/concerns/entity_concern.h"

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