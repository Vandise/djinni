#include "djinni/ecs/ecs.h"
#include "djinni/map/map.h"
#include "djinni/grid/grid.h"
#include "djinni/map/tile.h"
#include "djinni/geometry/point.h"

static void create_tile_collision_object(Djinni_Map* djinni_map, Djinni_MapTile* mt) {
  Djinni_MapCollisionDefinition* def = djinni_map->collision_definitions->data[mt->collision_index];
  Djinni_Grid* grid = djinni_grid_state_get_grid();

  for (int i = 0; i < def->shapes->used; i++) {
    DjinniEntityId id;
    Djinni_MapCollisionShape* shape = def->shapes->data[i];

    if (shape->type == COLLISION_DEFINITION_RECTANGLE) {    
      id = djinni_ecs_collision_concern_create_centered_child_rectangle(
        mt->x, mt->y,
        shape->width, shape->height,
        djinni_map->base_tile_grid_width, djinni_map->base_tile_grid_height,
        1
      );
      djinni_grid_insert(grid, id, djinni_grid_concern_compute_ring(id));
    }
  }
}

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
    mt->collision_index = -1;

    // convert flattened array to x-y coordinates
    mt->x = i % layer->tiles.nx_tiles;
    mt->y = i / layer->tiles.ny_tiles;

    // translate to isometric
    djinni_point_concern_point_to_iso(
      djinni_map->height,
      &(mt->x), &(mt->y),
      djinni_map->base_tile_grid_width, djinni_map->base_tile_grid_height
    );

    cJSON* collision_node = cJSON_GetObjectItem(tile_node, "collision.definition.id");
    if (collision_node != NULL) {
      mt->collision_index = collision_node->valueint;
      create_tile_collision_object(djinni_map, mt);
    }
  }
}