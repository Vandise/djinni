#include "djinni/ecs/components/position.h"

static Djinni_Position* states[DJINNI_MAX_STATES];
static int active_state;

void djinni_ecs_component_position_initialize(int base_entity_count) {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    states[i] = malloc(base_entity_count * sizeof(Djinni_Position));
  }
}

inline Djinni_Position* djinni_ecs_component_position_get(DjinniEntityId id) {
  return &(states[active_state][id]);
}

void djinni_ecs_component_position_pool_expand(int new_size) {
  states[active_state] = (realloc(
    states[active_state], new_size * sizeof(Djinni_Position)
  ));
}

void djinni_ecs_component_position_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    Djinni_Position* state = states[i];
    free(state);
    states[i] = NULL;
  }
}