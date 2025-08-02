#include "djinni/djinni.h"

void djinni_debug_draw_collision_box(DjinniEntityId id, double dt) {
  Djinni_Game_Camera* camera = djinni_game_camera_get_camera();
  Djinni_Collidable* collision_box = djinni_ecs_component_collision_get(id);
  Djinni_Position* position = djinni_ecs_component_position_get(id);

  SDL_Rect r = {
    position->x - camera->x,
    position->y - camera->y,
    collision_box->width,
    collision_box->height
  };

  SDL_SetRenderDrawColor(djinni_video_renderer(), 128, 128, 128, 255);
  SDL_RenderDrawRect(djinni_video_renderer(), &r);
  djinni_video_renderer_reset_draw_color();
}