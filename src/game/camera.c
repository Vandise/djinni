#include "djinni/game/camera.h"
#include "djinni/game/game.h"
#include "djinni/ecs/components/position.h"

static Djinni_Game_Camera states[DJINNI_MAX_STATES];
static int active_state;

void djinni_game_camera_initialize(Djinni_GameSettings settings) {
  active_state = 0;

  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    states[i] = (Djinni_Game_Camera){
      .x = settings.camera_settings.x,
      .y = settings.camera_settings.y,
      .width = settings.camera_settings.width,
      .height = settings.camera_settings.height,
      .zoom = settings.camera_settings.zoom,
      .follow = {
        .id = -1,
        .x_offset = 0,
        .y_offset = 0
      }
    };
  }
}

void djinni_game_camera_update() {
  Djinni_Game_Camera* camera = djinni_game_camera_get_camera();

  if (camera->follow.id >= 0) {
    Djinni_Position* entity_position = djinni_ecs_component_position_get(camera->follow.id);
    camera->x = (entity_position->x - (camera->width / (2 * camera->zoom))) + camera->follow.x_offset;
    camera->y = (entity_position->y - (camera->height / (2 * camera->zoom))) + camera->follow.y_offset;
  }
}

void djinni_game_camera_follow(DjinniEntityId id, int x_offset, int y_offset) {
  Djinni_Game_Camera* camera = djinni_game_camera_get_camera();
  camera->follow.id = id;
  camera->follow.x_offset = x_offset;
  camera->follow.y_offset = y_offset;

  djinni_game_camera_update();
}

int djinni_game_camera_entity_in_viewport(DjinniEntityId id) {
  Djinni_Game_Camera* camera = djinni_game_camera_get_camera();
  Djinni_Position* entity_position = djinni_ecs_component_position_get(id);

  int x1, x2, y1, y2;
  int rw = camera->width / camera->zoom;
  int rh = camera->height / camera->zoom;

  x1 = camera->x;
  x2 = x1 + rw;

  y1 = camera->y;
  y2 = y1 + rh;

  int x = entity_position->x;
  int y = entity_position->y;

  if (x < x1 || x > x2 || y < y1 || y > y2) {
    return 0;
  }

  return 1;
}

inline Djinni_Game_Camera* djinni_game_camera_get_camera() {
  return &(states[active_state]);
}

void djinni_game_camera_destroy() {}