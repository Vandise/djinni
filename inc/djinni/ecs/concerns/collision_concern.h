#ifndef DJINNI_ECS_CONCERNS_COLLISION
#define DJINNI_ECS_CONCERNS_COLLISION 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"

DjinniEntityId djinni_ecs_collision_concern_create_rectangle(int x, int y, int width, int height, int solid);

DjinniEntityId djinni_ecs_collision_concern_create_centered_child_rectangle(
  int x, int y, int width, int height, int parent_width, int parent_height, int solid
);

#endif