#ifndef DJINNI_MAP_LAYER
#define DJINNI_MAP_LAYER 1

#include <stddef.h>
#include "djinni/map/shared.h"
#include "djinni/util/json.h"
#include "djinni/map/tile.h"

typedef struct Djinni_MapLayerStruct {
  int id;

  struct Djinni_MapLayerTilesStruct {
    int n_tiles;
    int nx_tiles;
    int ny_tiles;
    int tile_width;
    int tile_height;

    Djinni_MapTile* data;
  } tiles;

  void (*draw)(double dt);
} Djinni_MapLayer;

void djinni_map_layer_load(Djinni_Map* djinni_map, cJSON* layer_node);

#endif