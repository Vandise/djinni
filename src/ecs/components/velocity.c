#include "djinni/ecs/components/velocity.h"

static Djinni_Velocity* states[DJINNI_MAX_STATES];
static int active_state;

void djinni_ecs_component_velocity_initialize(int base_entity_count) {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    states[i] = malloc(base_entity_count * sizeof(Djinni_Velocity));
  }
}

inline Djinni_Velocity* djinni_ecs_component_velocity_get(DjinniEntityId id) {
  return &(states[active_state][id]);
}

void djinni_ecs_component_velocity_pool_expand(int new_size) {
  states[active_state] = (realloc(
    states[active_state], new_size * sizeof(Djinni_Velocity)
  ));
}

void djinni_ecs_component_velocity_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    Djinni_Velocity* state = states[i];
    free(state);
    states[i] = NULL;
  }
}