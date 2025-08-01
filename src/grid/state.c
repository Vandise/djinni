#include "djinni/grid/grid.h"
#include "djinni/ecs/ecs.h"

static Djinni_Grid* states[DJINNI_MAX_STATES];
static int active_state;

void djinni_grid_state_initialize() {
  active_state = 0;
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    states[i] = NULL;
  }
}

void djinni_grid_state_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    if (states[i] != NULL) {
      djinni_grid_destroy(states[i]);
    }
  }
}