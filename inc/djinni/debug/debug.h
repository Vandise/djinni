#ifndef DJINNI_DEBUG_HELPERS
#define DJINNI_DEBUG_HELPERS 1

#include "djinni/ecs/shared.h"

void djinni_debug_draw_grid(double dt);
void djinni_debug_draw_collision_box(DjinniEntityId id, double dt);
void djinni_debug_draw_collision_boxes(double dt);

#endif