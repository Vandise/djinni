#ifndef DJINNI_MAP
#define DJINNI_MAP 1

#include "djinni/common.h"
#include "djinni/util/json.h"
#include "djinni/map/state.h"
#include "djinni/map/layer.h"

#define DJINNI_MAP_N_LAYERS 10
#define DJINNI_MAP_OCCLUSION_LAYER 4

typedef struct Djinni_MapStruct {
  char map_file_name[DJINNI_MAX_FILE_NAME];

  int width;
  int height;
  int base_tile_grid_width;
  int base_tile_grid_height;

  Djinni_MapLayer layers[DJINNI_MAP_N_LAYERS];
} Djinni_Map;

Djinni_Map* djinni_map_load(char* file_name);
void djinni_map_destroy(Djinni_Map* djinni_map);

#endif