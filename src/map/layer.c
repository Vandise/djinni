#include "djinni/map/map.h"
#include "djinni/map/layer.h"
#include "djinni/render/isometric_occlusion_layer.h"

void djinni_map_layer_load(Djinni_Map* djinni_map, cJSON* layer_node) {
  int layer_id = cJSON_GetObjectItem(layer_node, "layer")->valueint;

  Djinni_MapLayer* layer = &(djinni_map->layers[layer_id]);
    layer->id = layer_id;

  cJSON* tiles_node = cJSON_GetObjectItem(layer_node, "tiles");
  if (tiles_node != NULL) {
    djinni_map_tiles_load(djinni_map, layer, tiles_node);
  }

  if (layer->id == DJINNI_MAP_OCCLUSION_LAYER) {
    if (tiles_node != NULL) {
      for (int i = 0; i < layer->tiles.n_tiles; i++) {
        Djinni_MapTile* mt = &(layer->tiles.data[i]);
        djinni_render_isometric_occlusion_layer_insert_tile(mt->x, mt->y, mt->atlas_id, mt->tile_index);
      }
    }

    layer->draw = djinni_render_isometric_occlusion_layer_draw;
  }

  //
  // todo: other layer draw handlers
  //
}