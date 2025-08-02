#ifndef DJINNI_ECS
#define DJINNI_ECS 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"
#include "djinni/ecs/component.h"
#include "djinni/ecs/components/position.h"
#include "djinni/ecs/components/dirty.h"
#include "djinni/ecs/components/sprite.h"
#include "djinni/ecs/components/behavior.h"
#include "djinni/ecs/components/collision.h"

#include "djinni/ecs/systems/entity_draw_system.h"

typedef struct Djinni_ECSStruct {
  int* free_ids;
  int free_count;
  int n_used;
  int n_max_entities;
} Djinni_ECS;

void djinni_ecs_initialize(int base_entity_count);
DjinniEntityId djinni_ecs_create_entity(int mask);
void djinni_ecs_destroy_entity(DjinniEntityId id);
int djinni_ecs_pool_size();
void djinni_ecs_pool_expand();
void djinni_ecs_destroy();

#endif