#include "djinni/ecs/ecs.h"
#include "djinni/ecs/components/behavior.h"

static Djinni_Behavior* states[DJINNI_MAX_STATES];
static int pool_sizes[DJINNI_MAX_STATES];
static int active_state;

void djinni_ecs_component_behavior_initialize(int base_entity_count) {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    Djinni_Behavior* state = malloc(base_entity_count * sizeof(Djinni_Behavior));

    for (int j = 0; j < base_entity_count; j++) {
      state[j].update = NULL;
      state[j].draw = NULL;
      state[j].collide = NULL;
    }

    states[i] = state;
    pool_sizes[i] = base_entity_count;
  }
}

inline Djinni_Behavior* djinni_ecs_component_behavior_get(DjinniEntityId id) {
  return &(states[active_state][id]);
}

inline void djinni_ecs_component_behavior_add_draw(DjinniEntityId id, void (*draw)(DjinniEntityId self, double dt)) {
  states[active_state][id].draw = draw;
}

inline void djinni_ecs_component_behavior_add_update(DjinniEntityId id, void (*update)(DjinniEntityId self, double dt)) {
  states[active_state][id].update = update;
}

inline void djinni_ecs_component_behavior_add_collide(DjinniEntityId id, void (*collide)(DjinniEntityId self, DjinniEntityId other, double dt)) {
  states[active_state][id].collide = collide;
}

inline void djinni_ecs_component_behavior_add_enter_viewport(DjinniEntityId id, void (*enter_viewport)(DjinniEntityId self, double dt)) {
  states[active_state][id].enter_viewport = enter_viewport;
}

inline void djinni_ecs_component_behavior_add_exit_viewport(DjinniEntityId id, void (*exit_viewport)(DjinniEntityId self, double dt)) {
  states[active_state][id].exit_viewport = exit_viewport;
}

inline void djinni_ecs_component_behavior_add_destroy(DjinniEntityId id, void (*destroy)(DjinniEntityId self, double dt)) {
  states[active_state][id].destroy = destroy;
}

void djinni_ecs_component_behavior_pool_expand(int new_size) {
  int state_size = djinni_ecs_pool_size();

  states[active_state] = (realloc(
    states[active_state], new_size * sizeof(Djinni_Behavior)
  ));

  for (int i = state_size; i < new_size; i++) {
    states[active_state][i].update = NULL;
    states[active_state][i].draw = NULL;
    states[active_state][i].collide = NULL;
  }

  pool_sizes[active_state] = new_size;
}

void djinni_ecs_component_behavior_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    Djinni_Behavior* state = states[i];
    free(state);
    states[i] = NULL;
  }
}