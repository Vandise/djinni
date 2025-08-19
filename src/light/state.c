#include "djinni/light/state.h"

static DjinniArray* states[DJINNI_MAX_STATES];
static int active_state;

void djinni_light_state_initialize() {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    states[i] = djinni_array_initialize(2);
  }
}

inline void djinni_light_state_add_light(Djinni_Light* light) {
  djinni_array_insert(states[active_state], light);
}

inline DjinniArray* djinni_light_state_get_lights() {
  return states[active_state];
}

void djinni_light_state_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    if (states[i] != NULL) {
      djinni_array_destroy(states[i], free);
    }
  }
}