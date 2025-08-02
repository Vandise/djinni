#include "djinni/common.h"
#include "djinni/render/isometric_occlusion_layer.h"
#include "djinni/render/tile_layer.h"
#include "djinni/ecs/ecs.h"
#include "djinni/game/camera.h"
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
      djinni_array_remove_index(layer, i);
      free(drawable_entity);
      break;
    }
  }
}

void djinni_render_isometric_occlusion_layer_insert_tile(int x, int y, int atlas_id, int tile_index) {
  Djinni_Drawable* drawable_entity = malloc(sizeof(Djinni_Drawable));
    drawable_entity->type = DJINNI_DRAW_TILE;
    drawable_entity->tile.x = x;
    drawable_entity->tile.y = y;
    drawable_entity->tile.atlas_id = atlas_id;
    drawable_entity->tile.tile_index = tile_index;

  djinni_array_insert(isometric_occlusion_layer[active_state], drawable_entity);
}

void djinni_render_isometric_occlusion_layer_remove_tile(int x, int y) {
  Djinni_Drawable* drawable_entity = NULL;
  DjinniArray* layer = isometric_occlusion_layer[active_state];

  for (int i = 0; i < layer->used; i++) {
    drawable_entity = layer->data[i];
    if (drawable_entity->tile.x == x && drawable_entity->tile.y == y) {
      djinni_array_remove_index(layer, i);
      free(drawable_entity);
      break;
    }
  }
}

static int djinni_render_isometric_occlusion_layer_draw_comparator(const void *a, const void *b) {
  Djinni_Drawable* d1 = *(Djinni_Drawable**)a;
  Djinni_Drawable* d2 = *(Djinni_Drawable**)b;

  int d1y, d2y, d1x, d2x, d1w, d2w, result;

  if (d1->type == DJINNI_DRAW_ENTITY) {
    Djinni_Sprite* sprite = djinni_ecs_component_sprite_get(d1->entity_id);
    Djinni_Position* position = djinni_ecs_component_position_get(d1->entity_id);
    d1y = position->y;
    d1x = position->x;
    d1w = sprite->src.w;
  } else {
    d1y = d1->tile.y;
  }

  if (d2->type == DJINNI_DRAW_ENTITY) {
    Djinni_Sprite* sprite = djinni_ecs_component_sprite_get(d2->entity_id);
    Djinni_Position* position = djinni_ecs_component_position_get(d2->entity_id);
    d2y = position->y;
    d2x = position->x;
    d2w = sprite->src.w;
  } else {
    d2y = d2->tile.y;
  }

  result = d1y - d2y;

  if (result == 0) {
    result = (d1x + d1w) - (d2x + d2w);
  }

  return result;
}

void djinni_render_isometric_occlusion_layer_draw(int layer_id, double dt) {
  DjinniArray* layer = isometric_occlusion_layer[active_state];

  qsort(
    layer->data,
    layer->used,
    sizeof(void*),
    djinni_render_isometric_occlusion_layer_draw_comparator
  );

  for (int i = 0; i < layer->used; i++) {
    Djinni_Drawable* drawable_entity = layer->data[i];
    if (drawable_entity->type == DJINNI_DRAW_ENTITY && djinni_game_camera_entity_in_viewport(drawable_entity->entity_id)) {
      djinni_ecs_system_draw_entity(drawable_entity->entity_id, dt);
      continue;
    }

    if (
      drawable_entity->type == DJINNI_DRAW_TILE &&
      djinni_game_camera_point_in_viewport(drawable_entity->tile.x, drawable_entity->tile.y)
    ) {
      djinni_render_tile(drawable_entity, dt);
    }
  }
}

void djinni_render_isometric_occlusion_layer_destroy() {
  for (int i = 0; i < DJINNI_MAX_STATES; i++) {
    djinni_array_destroy(isometric_occlusion_layer[i], free);
  }
}