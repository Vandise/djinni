#include "djinni/ecs/ecs.h"

static Djinni_ECS* states[DJINNI_MAX_STATES];
static int active_state;

void djinni_ecs_initialize(int base_entity_count) {
  active_state = 0;

  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    Djinni_ECS* state = malloc(sizeof(Djinni_ECS));
      state->free_count = 0;
      state->n_used = 0;
      state->n_max_entities = base_entity_count;
      state->free_ids = malloc(base_entity_count * sizeof(DjinniEntityId));
    states[i] = state;
  }

  djinni_ecs_component_initialize(base_entity_count);
  djinni_ecs_component_position_initialize(base_entity_count);
  djinni_ecs_component_velocity_initialize(base_entity_count);
  djinni_ecs_component_dirty_initialize(base_entity_count);
  djinni_ecs_component_sprite_initialize(base_entity_count);
  djinni_ecs_component_behavior_initialize(base_entity_count);
  djinni_ecs_component_collision_initialize(base_entity_count);
}

DjinniEntityId djinni_ecs_create_entity(int mask) {
  int id;
  Djinni_ECS* state = states[active_state];

  if (state->free_count > 0) {
    id = state->free_ids[--state->free_count];
  } else {
    if (state->n_used >= state->n_max_entities) {
      djinni_ecs_pool_expand();
    }

    id = state->n_used++;
  }

  djinni_ecs_component_set(id, mask);

  return id;
}

void djinni_ecs_destroy_entity(DjinniEntityId id) {
  djinni_ecs_component_set(id, 0);

  djinni_ecs_component_sprite_destroy_entity(id);

  Djinni_ECS* state = states[active_state];
  state->free_ids[state->free_count++] = id;
}

inline int djinni_ecs_pool_size() {
  return states[active_state]->n_max_entities;
}

void djinni_ecs_pool_expand() {
  Djinni_ECS* state = states[active_state];

  int next_size = (state->n_max_entities * 3) / 2 + 8;

  djinni_ecs_component_pool_expand(next_size);
  djinni_ecs_component_position_pool_expand(next_size);
  djinni_ecs_component_velocity_pool_expand(next_size);
  djinni_ecs_component_dirty_pool_expand(next_size);
  djinni_ecs_component_sprite_pool_expand(next_size);
  djinni_ecs_component_behavior_pool_expand(next_size);
  djinni_ecs_component_collision_pool_expand(next_size);

  state->n_max_entities = next_size;
}

void djinni_ecs_destroy() {
  djinni_ecs_component_destroy();
  djinni_ecs_component_position_destroy();
  djinni_ecs_component_velocity_destroy();
  djinni_ecs_component_dirty_destroy();
  djinni_ecs_component_sprite_destroy();
  djinni_ecs_component_behavior_destroy();
  djinni_ecs_component_collision_destroy();

  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    Djinni_ECS* state = states[i];
    free(state->free_ids);
    free(state);
    states[i] = NULL;
  }
}