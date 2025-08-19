#include "djinni/djinni.h"

static Djinni_WindowSettings window_settings;
static Djinni_VideoSettings video_settings;
static Djinni_GameSettings game_settings;

void djinni_initialize(Djinni_WindowSettings ws, Djinni_VideoSettings vs, Djinni_GameSettings gs) {
  window_settings = ws;
  video_settings  = vs;
  game_settings   = gs;

  djinni_video_window_initialize(ws.name, ws.posx, ws.posy, ws.width, ws.height, ws.flags);
  djinni_video_renderer_initialize(vs.index, vs.rendererFlags);

  djinni_video_initialize(vs.videoFlags);

  djinni_render_isometric_occlusion_layer_initialize(gs.pool_settings.n_entities);

  djinni_ecs_initialize(gs.pool_settings.n_entities);
  djinni_map_state_initialize();
  djinni_light_state_initialize();
  djinni_grid_state_initialize();
  djinni_game_stage_initialize(gs.pool_settings.n_stages);
  djinni_game_camera_initialize(gs);
  djinni_game_initialize(gs);
  djinni_game_input_initialize();
}

void djinni_set_flag(const char *name, const char *value) {
  SDL_SetHint(name, value);
}

void djinni_start(int stage_id) {
  djinni_game_stage_change_stage(stage_id);
  djinni_game_runner_execute();
}

void djinni_destroy() {
  djinni_ecs_destroy();
  djinni_map_state_destroy();
  djinni_light_state_destroy();
  djinni_grid_state_destroy();
  djinni_game_stage_destroy();
  djinni_game_camera_destroy();
  djinni_game_destroy();
  djinni_game_input_destroy();
  djinni_render_isometric_occlusion_layer_destroy();
  djinni_light_destroy_shadow_texture();

  djinni_video_renderer_destroy();
  djinni_video_window_destroy();
}