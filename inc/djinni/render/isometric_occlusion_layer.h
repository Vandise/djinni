#ifndef DJINNI_RENDER_ISOMETRIC_OCCLUSION
#define DJINNI_RENDER_ISOMETRIC_OCCLUSION 1

#include "djinni/render/shared.h"
#include "djinni/ecs/shared.h"

typedef enum {
  DJINNI_DRAW_TILE,
  DJINNI_DRAW_ENTITY
} Djinni_DrawableType;

typedef struct Djinni_DrawableStruct {
  Djinni_DrawableType type;

  union {
    DjinniEntityId entity_id;

    struct {
      int x;
      int y;

      int atlas_id;
      int tile_index;
    } tile;
  };

} Djinni_Drawable;

void djinni_render_isometric_occlusion_layer_initialize(int base_size);
void djinni_render_isometric_occlusion_layer_insert_entity(DjinniEntityId id);
void djinni_render_isometric_occlusion_layer_remove_entity(DjinniEntityId id);
void djinni_render_isometric_occlusion_layer_destroy();

#endif