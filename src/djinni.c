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

  djinni_game_stage_initialize(gs.n_stages);
  djinni_game_initialize(gs);
  djinni_game_input_initialize();
}

void djinni_set_flag(const char *name, const char *value) {
  SDL_SetHint(name, value);
}

void djinni_destroy() {
  djinni_game_stage_destroy();
  djinni_game_destroy();
  djinni_game_input_destroy();

  djinni_video_renderer_destroy();
  djinni_video_window_destroy();
}