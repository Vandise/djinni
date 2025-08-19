#include "djinni/light/light.h"
#include "djinni/video/video.h"

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