#include "djinni/map/map.h"
#include "djinni/map/layer.h"

void djinni_map_layer_load(Djinni_Map* djinni_map, cJSON* layer_node) {
  int layer_id = cJSON_GetObjectItem(layer_node, "layer")->valueint;

  Djinni_MapLayer* layer = &(djinni_map->layers[layer_id]);
    layer->id = layer_id;

  cJSON* tiles_node = cJSON_GetObjectItem(layer_node, "tiles");
  if (tiles_node != NULL) {
    djinni_map_tiles_load(djinni_map, layer, tiles_node);
  }
}