#ifndef DJINNI_ECS_COMPONENT_POSITION
#define DJINNI_ECS_COMPONENT_POSITION 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"

typedef struct Djinni_PositionStruct {
  int x;
  int y;
} Djinni_Position;

void djinni_ecs_component_position_initialize(int base_entity_count);
Djinni_Position* djinni_ecs_component_position_get(DjinniEntityId id);
void djinni_ecs_component_position_pool_expand(int new_size);
void djinni_ecs_component_position_destroy();

#endif