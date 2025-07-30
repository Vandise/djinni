#include "djinni/common.h"
#include "djinni/render/isometric_occlusion_layer.h"
#include "djinni/util/array.h"

static DjinniArray* isometric_occlusion_layer[DJINNI_MAX_STATES];
static int active_state = 0;

void djinni_render_isometric_occlusion_layer_initialize(int base_size) {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    isometric_occlusion_layer[i] = djinni_array_initialize(base_size);
  }
}

void djinni_render_isometric_occlusion_layer_insert_entity(DjinniEntityId id) {
  Djinni_Drawable* drawable_entity = malloc(sizeof(Djinni_Drawable));
    drawable_entity->type = DJINNI_DRAW_ENTITY;
    drawable_entity->entity_id = id;

  djinni_array_insert(isometric_occlusion_layer[active_state], drawable_entity);
}

void djinni_render_isometric_occlusion_layer_remove_entity(DjinniEntityId id) {
  Djinni_Drawable* drawable_entity = NULL;
  DjinniArray* layer = isometric_occlusion_layer[active_state];

  for (int i = 0; i < layer->used; i++) {
    drawable_entity = layer->data[i];
    if (drawable_entity->entity_id == id) {
      djinni_array_removeIndex(layer, i);
      free(drawable_entity);
      break;
    }
  }
}

void djinni_render_isometric_occlusion_layer_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    djinni_array_destroy(isometric_occlusion_layer[i], free);
  }
}