#include "djinni/map/map.h"
#include "djinni/game/game.h"
#include "djinni/grid/grid.h"
#include "djinni/grid/state.h"
#include "djinni/util/file.h"

static void load_layers(Djinni_Map* djinni_map, cJSON* root) {
  cJSON* layers_node = cJSON_GetObjectItem(root, "layers");
  int n_layers = cJSON_GetArraySize(layers_node);

  for (int i = 0; i < n_layers; i++) {
    djinni_map_layer_load(djinni_map, cJSON_GetArrayItem(layers_node, i));
  }
}

static void load_grid(Djinni_Map* djinni_map) {
  Djinni_Game* game = djinni_game_get_game();

  Djinni_Grid* grid = djinni_grid_create(
    game->settings.grid_settings.cell_capacity,
    djinni_map->width,
    djinni_map->height,
    game->settings.grid_settings.finest_size,
    game->settings.grid_settings.medium_size,
    game->settings.grid_settings.coarse_size
  );

  djinni_grid_state_set_grid(grid);
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

    load_grid(djinni_map);

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