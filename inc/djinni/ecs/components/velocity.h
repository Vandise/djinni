#ifndef DJINNI_ECS_COMPONENT_VELOCITY
#define DJINNI_ECS_COMPONENT_VELOCITY 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"

typedef struct Djinni_VelocityStruct {
  int x;
  int y;
} Djinni_Velocity;

void djinni_ecs_component_velocity_initialize(int base_entity_count);
Djinni_Velocity* djinni_ecs_component_velocity_get(DjinniEntityId id);
void djinni_ecs_component_velocity_pool_expand(int new_size);
void djinni_ecs_component_velocity_destroy();

#endif