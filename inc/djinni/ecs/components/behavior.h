#ifndef DJINNI_ECS_COMPONENT_BEHAVIOR
#define DJINNI_ECS_COMPONENT_BEHAVIOR 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"

typedef struct Djinni_BehaviorStruct {
  double last_tick;

  void (*update)(DjinniEntityId self, double dt);
  void (*draw)(DjinniEntityId self, double dt);
  void (*collide)(DjinniEntityId self, DjinniEntityId other, double dt);
  void (*enter_viewport)(DjinniEntityId self, double dt);
  void (*exit_viewport)(DjinniEntityId self, double dt);
  void (*destroy)(DjinniEntityId self, double dt);
} Djinni_Behavior;

void djinni_ecs_component_behavior_initialize(int base_entity_count);

Djinni_Behavior* djinni_ecs_component_behavior_get(DjinniEntityId id);

void djinni_ecs_component_behavior_add_draw(DjinniEntityId id, void (*draw)(DjinniEntityId self, double dt));
void djinni_ecs_component_behavior_add_update(DjinniEntityId id, void (*update)(DjinniEntityId self, double dt));
void djinni_ecs_component_behavior_add_collide(DjinniEntityId id, void (*collide)(DjinniEntityId self, DjinniEntityId other, double dt));
void djinni_ecs_component_behavior_add_enter_viewport(DjinniEntityId id, void (*enter_viewport)(DjinniEntityId self, double dt));
void djinni_ecs_component_behavior_add_exit_viewport(DjinniEntityId id, void (*exit_viewport)(DjinniEntityId self, double dt));
void djinni_ecs_component_behavior_add_destroy(DjinniEntityId id, void (*destroy)(DjinniEntityId self, double dt));

void djinni_ecs_component_behavior_pool_expand(int new_size);
void djinni_ecs_component_behavior_destroy();

#endif