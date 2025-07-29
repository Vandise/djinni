#ifndef DJINNI_ECS_COMPONENT
#define DJINNI_ECS_COMPONENT 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"

void djinni_ecs_component_initialize(int base_entity_count);
void djinni_ecs_component_set(DjinniEntityId id, int mask);
int djinni_ecs_component_includes(DjinniEntityId id, int mask);
void djinni_ecs_component_pool_expand(int new_size);
void djinni_ecs_component_destroy();

#endif