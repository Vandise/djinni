#ifndef DJINNI_DEBUG_HELPERS
#define DJINNI_DEBUG_HELPERS 1

#include "djinni/ecs/shared.h"
#include "djinni/light/light.h"

void djinni_debug_draw_grid(double dt);
void djinni_debug_draw_collision_box(DjinniEntityId id, double dt);
void djinni_debug_draw_collision_boxes(double dt);
void djinni_debug_draw_rays(Djinni_Light *light, int tile_w, int tile_h, double dt);

#endif