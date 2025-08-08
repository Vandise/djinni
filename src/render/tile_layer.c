#include "djinni/render/tile_layer.h"
#include "djinni/map/state.h"
#include "djinni/map/map.h"
#include "djinni/game/camera.h"
#include "djinni/video/video.h"

static int tile_draw_comparator(const void *a, const void *b) {
  Djinni_MapTile *t1, *t2;

  t1 = (Djinni_MapTile*)a;
  t2 = (Djinni_MapTile*)b;

  return t1->y - t2->y;
}

static void draw_tile(int x, int y, int atlas_id, int tile_index, double dt) {
  Djinni_Map* djinni_map = djinni_map_state_get_map();
  Djinni_Game_Camera* camera = djinni_game_camera_get_camera();

  Djinni_AtlasImage* img = djinni_video_image_atlas_get_index(
    djinni_map->atlases->data[atlas_id],
    tile_index
  );

	SDL_Rect src;
  	src.x = img->x;
  	src.y = img->y;
  	src.w = img->w;
  	src.h = img->h;

	SDL_Rect dest;
  	dest.x = x - camera->x;
  	dest.y = y - camera->y;
  	dest.w = djinni_map->base_tile_grid_width;
  	dest.h = djinni_map->base_tile_grid_height;

	SDL_RenderCopy(djinni_video_renderer(), img->atlas->texture, &src, &dest);
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