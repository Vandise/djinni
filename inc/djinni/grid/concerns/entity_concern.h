#ifndef DJINNI_GRID_CONCERNS_ENTITY_CONCERN
#define DJINNI_GRID_CONCERNS_ENTITY_CONCERN 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"
#include "djinni/grid/shared.h"

DJINNI_GRID_RING djinni_grid_concern_compute_ring(DjinniEntityId id);
int djinni_grid_concern_entity_needs_cell_update(Djinni_Grid* grid, DjinniEntityId id);

#endif