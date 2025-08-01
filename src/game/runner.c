#include "djinni/djinni.h"

static void prepare() {
  djinni_video_renderer_clear();
  djinni_video_renderer_reset_draw_color();

  Djinni_GameStage* stage = djinni_game_stage_get_active_stage();
    stage->prepare(stage);
}

static void update(double dt) {
  djinni_game_camera_update();

  Djinni_GameStage* stage = djinni_game_stage_get_active_stage();
    stage->update(stage, dt);
}

static void draw(double dt) {
  Djinni_GameStage* stage = djinni_game_stage_get_active_stage();
    stage->draw(stage, dt);
}

static void present() {
  djinni_video_renderer_present();
}

static void lock_frame_rate(long *then, float *remainder, double maxfps) {
  long wait, frameTime;

  wait = 16 + *remainder;
  *remainder -= (int)*remainder;
  frameTime = SDL_GetTicks() - *then;
  wait -= frameTime;

  if (wait < 1) {
    wait = 1;
  }

  SDL_Delay(wait);

  *remainder += (1000/maxfps)/100;
  *then = SDL_GetTicks();
}

void djinni_game_runner_execute() {
  double dt        = 0.0;
  double tmpDelta  = 0.0;
  long   then      = 0;
  float  remainder = 0;
  int    fps       = 0;
  float  logic_rate = 0;
  float  fps_lock   = 0;
  long   nextFPS   = SDL_GetTicks() + 1000;

  Djinni_Game* game = djinni_game_get_game();
    logic_rate = game->settings.engine_settings.logic_rate;
    fps_lock = game->settings.engine_settings.fps_lock;

  while(!djinni_game_terminated()) {
    then = SDL_GetTicks();

    prepare();

    djinni_game_input_process();

    //
    // lock game logic to 60fps
    //
  	while (dt > 1) {
  		tmpDelta = dt;
  		dt = 1;
      update(then);
  		dt = (tmpDelta - 1);
  	}

    update(then);

    draw(then);

    present();

    SDL_Delay(1);

    dt = logic_rate * (SDL_GetTicks() - then);

    fps++;
    if (SDL_GetTicks() >= nextFPS) {
		  fps = 0;
		  nextFPS = SDL_GetTicks() + 1000;
	  }

    //
    // todo: lock_frame_rate(&then, &remainder, fps_lock);
    //       lock logic rate separate from the game loop
    //
  }
}