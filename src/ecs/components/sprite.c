#include "djinni/ecs/ecs.h"

static Djinni_Sprite* states[DJINNI_MAX_STATES];
static int pool_sizes[DJINNI_MAX_STATES];
static int active_state;

void djinni_ecs_component_sprite_initialize(int base_entity_count) {
  active_state = 0;

  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    Djinni_Sprite* state = malloc(base_entity_count * sizeof(Djinni_Sprite));

    for (int j = 0; j < base_entity_count; j++) {
      state[j].texture_status = 0;
      state[j].texture = NULL;
    }

    states[i] = state;
    pool_sizes[i] = base_entity_count;
  }
}

inline Djinni_Sprite* djinni_ecs_component_sprite_get(DjinniEntityId id) {
  return &(states[active_state][id]);
}

void djinni_ecs_component_sprite_pool_expand(int new_size) {
  int state_size = djinni_ecs_pool_size();

  states[active_state] = (realloc(
    states[active_state], new_size * sizeof(Djinni_Sprite)
  ));

  for (int i = state_size; i < new_size; i++) {
    states[active_state][i].texture_status = 0;
    states[active_state][i].texture = NULL;
  }

  pool_sizes[active_state] = new_size;
}

void djinni_ecs_component_sprite_destroy_entity(DjinniEntityId id) {
  Djinni_Sprite* state = states[active_state];

  if (state[id].texture != NULL) {
    SDL_DestroyTexture(state[id].texture);
    state[id].texture = NULL;
  }
}

void djinni_ecs_component_sprite_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    Djinni_Sprite* state = states[i];

    for (int j = 0; j < pool_sizes[i]; j++) {
      if (state[j].texture != NULL) {
        SDL_DestroyTexture(state[j].texture);
      }
    }

    free(state);
    states[i] = NULL;
  }
}