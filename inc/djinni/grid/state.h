#ifndef DJINNI_GRID_STATE
#define DJINNI_GRID_STATE 1

#include "djinni/grid/shared.h"

void djinni_grid_state_initialize();
Djinni_Grid* djinni_grid_state_get_grid();
void djinni_grid_state_set_grid(Djinni_Grid* grid);
void djinni_grid_state_destroy();

#endif