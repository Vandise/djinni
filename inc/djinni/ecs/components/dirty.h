#ifndef DJINNI_ECS_COMPONENT_DIRTY
#define DJINNI_ECS_COMPONENT_DIRTY 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"

void djinni_ecs_component_dirty_initialize(int base_entity_count);
void djinni_ecs_component_dirty_flag(DjinniEntityId id);
int djinni_ecs_component_dirty_is_dirty(DjinniEntityId id);
void djinni_ecs_component_dirty_clear(DjinniEntityId id);
void djinni_ecs_component_dirty_pool_expand(int new_size);
void djinni_ecs_component_dirty_destroy();

#endif