#include "djinni/ecs/component.h"

static int* states[DJINNI_MAX_STATES];
static int active_state;

void djinni_ecs_component_initialize(int base_entity_count) {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    int* state = malloc(base_entity_count * sizeof(DjinniEntityId));
    states[i] = state;
  }
}

inline void djinni_ecs_component_set(DjinniEntityId id, int mask) {
  states[active_state][id] = mask;
}

inline int djinni_ecs_component_includes(DjinniEntityId id, int mask) {
  return (states[active_state][id] & mask) == mask;
}

void djinni_ecs_component_pool_expand(int new_size) {
  states[active_state] = (realloc(
    states[active_state], new_size * sizeof(DjinniEntityId)
  ));
}

void djinni_ecs_component_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    int* state = states[i];
    free(state);
    states[i] = NULL;
  }
}