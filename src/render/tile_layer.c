#include "djinni/render/tile_layer.h"
#include "djinni/map/state.h"
#include "djinni/map/map.h"
#include "djinni/game/camera.h"
#include "djinni/video/video.h"

/*

  TODO: this is hard-coded until querying the map atlas

  todo: only qsort if layer is changed
*/

static SDL_Texture* texture = NULL;

static int tile_draw_comparator(const void *a, const void *b) {
  Djinni_MapTile *t1, *t2;

  t1 = (Djinni_MapTile*)a;
  t2 = (Djinni_MapTile*)b;

  return t1->y - t2->y;
}

static void draw_tile(int x, int y, int atlas_id, int tile_index, double dt) {
  Djinni_Game_Camera* camera = djinni_game_camera_get_camera();

  if (texture == NULL) {
    texture = IMG_LoadTexture(djinni_video_renderer(), "bin/gfx/greycube.png");
  }

  SDL_Rect dest;
    dest.x = x - camera->x;
    dest.y = y - camera->y;
    dest.w = 62;
    dest.h = 63;

  SDL_RenderCopy(djinni_video_renderer(), texture, NULL, &dest);
}

void djinni_render_tile(Djinni_Drawable* drawable_entity, double dt) {
  draw_tile(
    drawable_entity->tile.x,
    drawable_entity->tile.y,
    drawable_entity->tile.atlas_id,
    drawable_entity->tile.tile_index,
    dt
  );
}

void djinni_render_tile_layer_draw(int layer_id, double dt) {
  Djinni_Map* djinni_map = djinni_map_state_get_map();
  Djinni_MapLayer* layer = &(djinni_map->layers[layer_id]);

  if (layer->dirty) {
    qsort(
      layer->tiles.data,
      layer->tiles.n_tiles,
      sizeof(Djinni_MapTile),
      tile_draw_comparator
    );
  }

  for (int i = 0; i < layer->tiles.n_tiles; i++) {
    Djinni_MapTile tile = layer->tiles.data[i];

    if (!tile.empty && djinni_game_camera_point_in_viewport(tile.x, tile.y)) {
      draw_tile(tile.x, tile.y, tile.atlas_id, tile.tile_index, dt);
    }
  }
}