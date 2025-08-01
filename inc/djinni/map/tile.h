#ifndef DJINNI_MAP_TILE
#define DJINNI_MAP_TILE 1

#include <stddef.h>
#include "djinni/map/shared.h"
#include "djinni/util/json.h"

/*
  tiles are an array of objects with an index and atlas coordinates
  tiles for a specific coordinate may also be empty with a 0

  tiles: Array(Object)
    Object:
      index(int): spritesheet atlas index
      atlas(int): index of the spritesheet atlas in the layer

  Example:
    "tiles":[{"index":2,"atlas":0},0]
*/
typedef struct Djinni_MapTileStruct {
  int empty;

  int layer;
  int tile_index;
  int atlas_id;

  int x;
  int y;
} Djinni_MapTile;

void djinni_map_tiles_load(Djinni_Map* djinni_map, Djinni_MapLayer* layer, cJSON* tiles_node);

#endif