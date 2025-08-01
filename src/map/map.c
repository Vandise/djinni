#include "djinni/map/map.h"
#include "djinni/util/file.h"

static void load_layers(Djinni_Map* djinni_map, cJSON* root) {
  cJSON* layers_node = cJSON_GetObjectItem(root, "layers");
  int n_layers = cJSON_GetArraySize(layers_node);

  for (int i = 0; i < n_layers; i++) {
    djinni_map_layer_load(djinni_map, cJSON_GetArrayItem(layers_node, i));
  }
}

Djinni_Map* djinni_map_load(char* file_name) {
  char* text = djinni_read_file(file_name);
  Djinni_Map* djinni_map = malloc(sizeof(Djinni_Map));

  if (text != NULL) {
    cJSON* root = cJSON_Parse(text);

    djinni_map->width = cJSON_GetObjectItem(root, "width")->valueint;
    djinni_map->height = cJSON_GetObjectItem(root, "height")->valueint;
    djinni_map->base_tile_grid_width = cJSON_GetObjectItem(root, "base_tile_grid_width")->valueint;
    djinni_map->base_tile_grid_height = cJSON_GetObjectItem(root, "base_tile_grid_height")->valueint;

    for (int i = 0; i < DJINNI_MAP_N_LAYERS; i++) {
      djinni_map->layers[i].id = -1;
      djinni_map->layers[i].tiles.data = NULL;
      djinni_map->layers[i].draw = NULL;
    }

    load_layers(djinni_map, root);

    cJSON_Delete(root);
  }

  free(text);

  return djinni_map;
}

void djinni_map_destroy(Djinni_Map* djinni_map) {
  for (int i = 0; i < DJINNI_MAP_N_LAYERS; i++) {
    if (djinni_map->layers[i].tiles.data != NULL) {
      free(djinni_map->layers[i].tiles.data);
    }
  }
  free(djinni_map);
}