#include "djinni/djinni.h"

static double next_layer_dt = 0;
static int draw_layer = 0;

void djinni_debug_draw_grid(double dt) {
  if (dt - next_layer_dt > 2000) {
    draw_layer++;
    if (draw_layer > 2) { draw_layer = 0; }
    next_layer_dt = dt;
  }

  Djinni_Game_Camera* camera = djinni_game_camera_get_camera();
  Djinni_Grid* grid = djinni_grid_state_get_grid();

  for (int level_index = draw_layer; level_index < draw_layer + 1; level_index++) {
    Djinni_GridLevel* level = &grid->levels[level_index];
    int grid_cell_width = level->cell_size;

    for (int y = 0; y < level->height; y++) {
      for (int x = 0; x < level->width; x++) {

        Djinni_GridCell* cell = &level->cells[y * level->width + x];

        SDL_Rect r = {
          (x * grid_cell_width) - camera->x,
          (y * grid_cell_width) - camera->y,
          grid_cell_width,
          grid_cell_width
        };

        if (cell->entities->used > 0) {
          SDL_SetRenderDrawColor(djinni_video_renderer(), 255, 255, 0, 255);
        } else {
          SDL_SetRenderDrawColor(djinni_video_renderer(), 128, 128, 128, 255);
        }

        SDL_RenderDrawRect(djinni_video_renderer(), &r);
      }
    }
  }
  djinni_video_renderer_reset_draw_color();
}