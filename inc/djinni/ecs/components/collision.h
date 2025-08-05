#ifndef DJINNI_ECS_COMPONENT_COLLISION
#define DJINNI_ECS_COMPONENT_COLLISION 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"
#include "djinni/grid/grid.h"

typedef struct Djinni_CollidableStruct {
  int width;
  int height;
  int solid;

  int parent_x_offset;
  int parent_y_offset;

  double last_collision_frame;

  Djinni_GridLocation grid_cache;
} Djinni_Collidable;

void djinni_ecs_component_collision_initialize(int base_entity_count);
Djinni_Collidable* djinni_ecs_component_collision_get(DjinniEntityId id);
void djinni_ecs_component_collision_pool_expand(int new_size);
void djinni_ecs_component_collision_destroy();

#endif