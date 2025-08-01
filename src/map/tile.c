#include "djinni/map/map.h"
#include "djinni/map/tile.h"
#include "djinni/geometry/point.h"

void djinni_map_tiles_load(Djinni_Map* djinni_map, Djinni_MapLayer* layer, cJSON* tiles_node) {
  layer->tiles.n_tiles = cJSON_GetArraySize(tiles_node);
  layer->tiles.nx_tiles = djinni_map->width / djinni_map->base_tile_grid_width;
  layer->tiles.ny_tiles = djinni_map->height / djinni_map->base_tile_grid_height;
  layer->tiles.data = malloc(sizeof(Djinni_MapTile) * layer->tiles.nx_tiles * layer->tiles.ny_tiles);

  for (int i = 0; i < layer->tiles.n_tiles; i++) {
    cJSON* tile_node = cJSON_GetArrayItem(tiles_node, i);
    Djinni_MapTile* mt = &(layer->tiles.data[i]);

    if (tile_node->type != cJSON_Object) {
      mt->empty = 1;
      continue;
    }

    mt->layer = layer->id;
    mt->empty = 0;
    mt->tile_index = cJSON_GetObjectItem(tile_node, "index")->valueint;
    mt->atlas_id = cJSON_GetObjectItem(tile_node, "atlas")->valueint;

    // convert flattened array to x-y coordinates
    mt->x = i % layer->tiles.nx_tiles;
    mt->y = i / layer->tiles.ny_tiles;

    // translate to isometric
    djinni_point_concern_point_to_iso(
      djinni_map->height,
      &(mt->x), &(mt->y),
      djinni_map->base_tile_grid_width, djinni_map->base_tile_grid_height
    );
  }
}