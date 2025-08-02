#ifndef DJINNI_RENDER_TILE
#define DJINNI_RENDER_TILE 1

#include "djinni/map/shared.h"
#include "djinni/render/shared.h"

void djinni_render_tile(Djinni_Drawable* drawable_entity, double dt);
void djinni_render_tile_layer_draw(int layer_id, double dt);

#endif