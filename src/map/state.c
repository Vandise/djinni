#include "djinni/map/map.h"
#include "djinni/map/state.h"

static Djinni_Map* states[DJINNI_MAX_STATES];
static int active_state;

void djinni_map_state_initialize() {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    states[i] = NULL;
  }
}

void djinni_map_state_set_map(Djinni_Map* djinni_map) {
  states[active_state] = djinni_map;
}

Djinni_Map* djinni_map_state_get_map() {
  return states[active_state];
}

void djinni_map_state_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    if (states[i] != NULL) {

    }
  }
}