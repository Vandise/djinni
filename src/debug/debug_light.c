#include "djinni/djinni.h"

void djinni_debug_draw_rays(Djinni_Light *light, int tile_w, int tile_h, double dt) {
  Djinni_Game_Camera* camera = djinni_game_camera_get_camera();
  Djinni_Map* djinni_map = djinni_map_state_get_map();

  SDL_SetRenderDrawColor(djinni_video_renderer(), 255, 0, 0, 64);

  float half_arc = light->penumbra_arc_degrees * (M_PI / 180.0f) * 0.5f;

  for (int r = 0; r < light->n_rays; r++) {
    // Main ray angle
    float base_angle = (r / (float)light->n_rays) * 2.0f * M_PI;

    for (int s = 0; s < light->n_sub_rays; s++) {
      // Spread sub-rays evenly in the penumbra arc
      float t = (s / (float)(light->n_sub_rays - 1)) - 0.5f; // -0.5 to +0.5
      float jitter = t * 2.0f * half_arc;
      float angle = base_angle + jitter;

      // End point of debug ray (scale to map size)
      int length = djinni_map->width; // pixels, just for visualization
      int x2 = (int)((light->x - camera->x) + cosf(angle) * length);
      int y2 = (int)((light->y - camera->y) + sinf(angle) * length);

      // Draw ray from light to endpoint
      SDL_RenderDrawLine(djinni_video_renderer(), light->x - camera->x, light->y - camera->y, x2, y2);
    }
  }

  djinni_video_renderer_reset_draw_color();
}
