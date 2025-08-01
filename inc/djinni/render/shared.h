#ifndef DJINNI_RENDER_DRAWABLE_SHARED
#define DJINNI_RENDER_DRAWABLE_SHARED 1

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

#endif