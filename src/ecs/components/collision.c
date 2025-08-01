#include "djinni/ecs/ecs.h"
#include "djinni/ecs/components/collision.h"

static Djinni_Collidable* states[DJINNI_MAX_STATES];
static int pool_sizes[DJINNI_MAX_STATES];
static int active_state;

void djinni_ecs_component_collision_initialize(int base_entity_count) {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    states[i] = malloc(base_entity_count * sizeof(Djinni_Collidable));

    for (int j = 0; j < base_entity_count; j++) {
      states[i][j].grid_cache.level = -1;
      states[i][j].grid_cache.cells = NULL;
    }

    pool_sizes[i] = base_entity_count;
  }
}

inline Djinni_Collidable* djinni_ecs_component_collision_get(DjinniEntityId id) {
  return &(states[active_state][id]);
}

void djinni_ecs_component_collision_pool_expand(int new_size) {
  int state_size = djinni_ecs_pool_size();

  states[active_state] = (realloc(
    states[active_state], new_size * sizeof(Djinni_Collidable)
  ));

  for (int i = state_size; i < new_size; i++) {
    states[active_state][i].grid_cache.level = -1;
    states[active_state][i].grid_cache.cells = NULL;
  }

  pool_sizes[active_state] = new_size;
}

void djinni_ecs_component_collision_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    Djinni_Collidable* state = states[i];

    for (int j = 0; j < pool_sizes[i]; j++) {
      if (states[i][j].grid_cache.cells != NULL) {
        // grid deletes cells
        djinni_array_destroy(states[i][j].grid_cache.cells, NULL);
      }
    }

    free(state);
    states[i] = NULL;
  }
}