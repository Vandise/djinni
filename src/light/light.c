#include "djinni/light/light.h"
#include "djinni/video/video.h"
#include "djinni/map/map.h"
#include "djinni/game/camera.h"

static SDL_Texture* shadow_texture;

inline SDL_Texture* djinni_light_get_shadow_texture() {
  return shadow_texture;
}

inline void djinni_light_destroy_shadow_texture() {
  SDL_DestroyTexture(shadow_texture);
  shadow_texture = NULL;
}

void djinni_light_generate_shadow_texture(int tile_width, int tile_height) {
  shadow_texture = SDL_CreateTexture(
    djinni_video_renderer(),
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_TARGET,
    tile_width, tile_height
  );

  if (!shadow_texture) {
    return;
  }

  SDL_SetTextureBlendMode(shadow_texture, SDL_BLENDMODE_BLEND);

  // Render into the texture
  SDL_SetRenderTarget(djinni_video_renderer(), shadow_texture);

  // Clear to transparent
  SDL_SetRenderDrawBlendMode(djinni_video_renderer(), SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(djinni_video_renderer(), 0, 0, 0, 0);
  SDL_RenderClear(djinni_video_renderer());

  // Draw the diamond filled with white
  SDL_SetRenderDrawColor(djinni_video_renderer(), 255, 255, 255, 255);

  int sx = 0, sy = 0;
  for (int y0 = 0; y0 < tile_height; ++y0) {
    int row_y = sy + y0;
    int x_l, x_r;
  
    if (y0 < tile_height/2) {
      float t = (float)y0 / (float)(tile_height/2);
      x_l = (int)(sx + tile_width/2 - (tile_width/2)*t);
      x_r = (int)(sx + tile_width/2 + (tile_width/2)*t);
    } else {
      float t2 = (float)(y0 - tile_height/2) / (float)(tile_height/2);
      x_l = (int)(sx + (tile_width/2)*t2);
      x_r = (int)(sx + tile_width - (tile_width/2)*t2);
    }

    SDL_RenderDrawLine(djinni_video_renderer(), x_l, row_y, x_r, row_y);
  }

  SDL_SetRenderTarget(djinni_video_renderer(), NULL);
}

static inline float subray_offset(int sub, int sub_count) {
  return ((sub + 0.5f) / (float)sub_count) - 0.5f;
}

static int get_blocker_level(Djinni_Map* djinni_map, int tx, int ty, int nx_tiles, int ny_tiles) {
  if (tx < 0 || ty < 0 || tx >= nx_tiles || ty >= ny_tiles) {
    return 1;
  }

  for (int layer_id = (DJINNI_MAP_N_LAYERS - 1); layer_id >= DJINNI_MAP_OCCLUSION_LAYER; layer_id--) {
    Djinni_MapLayer* layer = &(djinni_map->layers[layer_id]);

    // layer is not initialized / used
    if (layer->id < 0) { continue; }

    Djinni_MapTile* mt = &(layer->tiles.data[ty * nx_tiles + tx]);

    if (!mt->empty) {
      return layer->id;
    }
  }

  return 0;
}

static void apply_shadows(Djinni_Map* djinni_map, Djinni_Light* light, float dirx, float diry) {
  int x = light->x;
  int y = light->y;
  int nx_tiles = djinni_map->width / djinni_map->base_tile_grid_width;
  int ny_tiles = djinni_map->height / djinni_map->base_tile_grid_height;

  float dx = dirx, dy = diry;
  float tDeltaX = dx != 0.0f ? fabsf(1.0f / dx) : 1e30f;
  float tDeltaY = dy != 0.0f ? fabsf(1.0f / dy) : 1e30f;

  int stepX = (dx > 0) ? 1 : -1;
  int stepY = (dy > 0) ? 1 : -1;

  float fracX = light->x - floorf(light->x);
  float fracY = light->y - floorf(light->y);
  float tMaxX = (dx > 0) ? (1.0f - fracX) * tDeltaX : (fracX) * tDeltaX;
  float tMaxY = (dy > 0) ? (1.0f - fracY) * tDeltaY : (fracY) * tDeltaY;

  int steps = 0;
  int hit_blocker = 0;
  int layer_multiplier = 1;
  float darkness = 0.0f;
  float tiles_since_block = 0.0f;

  while (steps++ < light->max_daa_steps) {
    if (tMaxX < tMaxY) {
      tMaxX += tDeltaX;
      x += stepX;
    } else {
      tMaxY += tDeltaY;
      y += stepY;
    }

    if (x < 0 || y < 0 || x >= nx_tiles || y >= ny_tiles) {
      continue;
    }

    if (!hit_blocker) {
      hit_blocker = get_blocker_level(djinni_map, x, y, nx_tiles, ny_tiles);

      if (hit_blocker) {
        darkness = 0.0f;
        tiles_since_block = 0.0f;
        layer_multiplier = (hit_blocker - DJINNI_MAP_OCCLUSION_LAYER + 1);
        continue;
      }
    } else {
      tiles_since_block += 1.0f;

      float range_factor = 1.0f - (tiles_since_block / light->light_range);
      if (range_factor < 0.0f) {
        range_factor = 0.0f;
      }

      darkness += (light->shadow_steps * layer_multiplier) * range_factor;
      if (darkness > 1.0f) {
        darkness = 1.0f;
      }

      // Find the tile under the blocker layer and set the shadow
      for (int layer_id = (hit_blocker - 1); layer_id >= 0; layer_id--) {
        Djinni_MapLayer* layer = &(djinni_map->layers[layer_id]);

        if (layer->id < 0) {
          continue;
        }

        Djinni_MapTile* mt = &(layer->tiles.data[y * nx_tiles + x]);

        if (!mt->empty) {
          int v = (int)(darkness * light->max_alpha);
          mt->shadow_alpha = (mt->shadow_alpha > v) ? mt->shadow_alpha : v;

          printf("\t layer: %d :: alpha: %d at x: %d y: %d\n", layer->id, mt->shadow_alpha, x, y);
          break;
        }
      }

      if (range_factor <= 0.0f) {
        break;
      }
    }
  }
}

void gaussian_blur_shadows(Djinni_Map* djinni_map, Djinni_Light* light) {
  int nx_tiles = djinni_map->width / djinni_map->base_tile_grid_width;
  int ny_tiles = djinni_map->height / djinni_map->base_tile_grid_height;

  // iterate from the top-left to bottom-right to blur in the direction of the light.
  for (int y = 0; y < ny_tiles; y++) {
    for (int x = 0; x < nx_tiles; x++) {
      // Find the current tile and its top-left neighbor
      Djinni_MapTile* current_tile = NULL;
      Djinni_MapTile* neighbor_tile = NULL;
      
      // Iterate through all shadow layers
      for (int layer_id = 0; layer_id < DJINNI_MAP_N_LAYERS; layer_id++) {
        Djinni_MapLayer* layer = &(djinni_map->layers[layer_id]);
        if (layer->id < 0 || layer->id >= DJINNI_MAP_OCCLUSION_LAYER) {
          continue;
        }

        int current_idx = y * nx_tiles + x;
        current_tile = &(layer->tiles.data[current_idx]);

        // Get neighbor to the top and left
        if (x > 0 && y > 0) {
          int neighbor_idx = (y - 1) * nx_tiles + (x - 1);
          neighbor_tile = &(layer->tiles.data[neighbor_idx]);
          
          // Propagate the shadow darkness from the neighbor
          int new_alpha = (int)((float)neighbor_tile->shadow_alpha * 0.5f + (float)current_tile->shadow_alpha * 0.5f);
          current_tile->shadow_alpha = (unsigned char)fminf(new_alpha, light->max_alpha);
        }
      }
    }
  }
}

void djinni_light_generate_shadows(Djinni_Map* djinni_map, Djinni_Light* light) {
    const float arc_rad = light->penumbra_arc_degrees * (float)M_PI / 180.0f;
    
    // Convert light isometric position to Cartesian grid position
    // (This is the inverse of the isometric projection used for rendering)
    float iso_x = (float)light->x;
    float iso_y = (float)light->y;
    
    //
    // TODO: inverse ISO for light source
    // Assuming base_tile_grid_width and base_tile_grid_height are W and H
    //
    float W = djinni_map->base_tile_grid_width / 2.0f;
    float H = djinni_map->base_tile_grid_height / 4.0f;
    float cart_x = 0.5f * (iso_x / W - ((djinni_map->height / 2.0f) - iso_y) / H);
    float cart_y = 0.5f * (iso_x / W + ((djinni_map->height / 2.0f) - iso_y) / H);
    
    // Cartesian coordinates
    // light->x and light->y
    //
    int original_x = light->x;
    int original_y = light->y;
    light->x = (int)roundf(cart_x);
    light->y = (int)roundf(cart_y);

    for (int i = 0; i < light->n_rays; ++i) {
      float base_angle = (float)i * 2.0f * (float)M_PI / (float)light->n_rays;
      for (int s = 0; s < light->n_sub_rays; ++s) {
        float off = subray_offset(s, light->n_sub_rays);
        float a = base_angle + off * arc_rad;
        float ax = cosf(a);
        float ay = sinf(a);
        apply_shadows(djinni_map, light, ax, ay);
      }
    }

    gaussian_blur_shadows(djinni_map, light);

    // Restore original light coordinates
    light->x = original_x;
    light->y = original_y;
}
