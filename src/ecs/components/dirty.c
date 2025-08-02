#include "djinni/ecs/components/dirty.h"

static int* states[DJINNI_MAX_STATES];
static int active_state;

void djinni_ecs_component_dirty_initialize(int base_entity_count) {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    int* state = malloc(base_entity_count * sizeof(DjinniEntityId));
    states[i] = state;
  }
}

inline void djinni_ecs_component_dirty_flag(DjinniEntityId id) {
  states[active_state][id] = 1;
}

inline void djinni_ecs_component_dirty_clear(DjinniEntityId id) {
  states[active_state][id] = 0;
}

inline int djinni_ecs_component_dirty_is_dirty(DjinniEntityId id) {
  return (states[active_state][id]);
}

void djinni_ecs_component_dirty_pool_expand(int new_size) {
  states[active_state] = (realloc(
    states[active_state], new_size * sizeof(DjinniEntityId)
  ));
}

void djinni_ecs_component_dirty_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    int* state = states[i];
    free(state);
    states[i] = NULL;
  }
}