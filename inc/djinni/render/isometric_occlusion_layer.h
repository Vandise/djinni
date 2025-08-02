#ifndef DJINNI_RENDER_ISOMETRIC_OCCLUSION
#define DJINNI_RENDER_ISOMETRIC_OCCLUSION 1

#include "djinni/render/shared.h"

void djinni_render_isometric_occlusion_layer_initialize(int base_size);
void djinni_render_isometric_occlusion_layer_draw(int layer_id, double dt);
void djinni_render_isometric_occlusion_layer_insert_entity(DjinniEntityId id);
void djinni_render_isometric_occlusion_layer_remove_entity(DjinniEntityId id);
void djinni_render_isometric_occlusion_layer_insert_tile(int x, int y, int atlas_id, int tile_index);
void djinni_render_isometric_occlusion_layer_remove_tile(int x, int y);
void djinni_render_isometric_occlusion_layer_destroy();

#endif