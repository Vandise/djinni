#include "djinni/map/map.h"

static Djinni_Map* states[DJINNI_MAX_STATES];
static int active_state;

void djinni_map_state_initialize() {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    states[i] = NULL;
  }
}

void djinni_map_state_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    if (states[i] != NULL) {

    }
  }
}