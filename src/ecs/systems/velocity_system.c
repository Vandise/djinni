#include "djinni/ecs/ecs.h"

void djinni_ecs_system_velocity(DjinniEntityId id, double dt) {
  Djinni_Velocity* velocity = djinni_ecs_component_velocity_get(id);
  Djinni_Position* position = djinni_ecs_component_position_get(id);

  if (velocity->x != 0 || velocity->y != 0) {
    position->x += velocity->x;
    position->y += velocity->y;

    djinni_ecs_component_dirty_flag(id);
  }
}