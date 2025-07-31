#include "djinni/ecs/ecs.h"
#include "djinni/game/camera.h"
#include "djinni/video/video.h"
#include "djinni/ecs/systems/entity_draw_system.h"

void djinni_ecs_system_draw_entity(DjinniEntityId id) {
  Djinni_Game_Camera* camera = djinni_game_camera_get_camera();
  Djinni_Sprite* sprite = djinni_ecs_component_sprite_get(id);
  Djinni_Position* position = djinni_ecs_component_position_get(id);

  SDL_Rect dest;
    dest.x = position->x - camera->x;
    dest.y = position->y - camera->y;
    dest.w = sprite->src.w;
    dest.h = sprite->src.h;

  SDL_RenderCopy(djinni_video_renderer(), sprite->texture, NULL, &dest);
}