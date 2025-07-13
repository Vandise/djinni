#include "djinni/util/util.h"
#include "djinni/djinni.h"

static void prepare(Game* game) {
  game->activeStage->prepare(game->activeStage, game);

  Djinni.Video->Renderer->setDrawColor(Djinni.renderer, Djinni.renderer->backgroundColor);
  Djinni.Video->Renderer->clear(Djinni.renderer);
}

static void logic(Game* game, double dt) {

}

static void draw(Game* game, double dt) {
  for (int i = 0; i < game->world->entities->used; i++) {
    Entity* entity = (Entity*)(game->world->entities->data[i]);
    Djinni.Renderable->draw(Djinni.renderer, entity);
    game->activeStage->draw(game->activeStage, game, dt);
  }
}

static void present(Game* game) {
  Djinni.Video->Renderer->present(Djinni.renderer);
}

static void lockFrameRate(long *then, float *remainder, double maxfps) {
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

static void execute(Game* game) {
  Djinni_Util_Logger.log_debug("Djinni::Game::Runner.execute( game:(%p) )", game);

  double dt        = 0.0;
  double tmpDelta  = 0.0;
  long   then      = 0;
  float  remainder = 0;
  int    fps       = 0;
  long   nextFPS   = SDL_GetTicks() + 1000;

  /*
    1. prepare (bg color, clear)
    2. User IO
    3. Game logic
        Update, Check for collisions
    4. Draw (stage assets, entities)
    5. Present to window
  */

  while(game->terminated == 0) {
    then = SDL_GetTicks();

    prepare(game);
    Djinni.Game->Input->process(game);

    // lock logic to 60fps
  	while (dt > 1) {
  		tmpDelta = dt;
  		dt = 1;
      logic(game, tmpDelta);
  		dt = (tmpDelta - 1);
  	}

    logic(game, tmpDelta);

    draw(game, dt);
    present(game);

    SDL_Delay(1);

    dt = game->settings.logicRate * (SDL_GetTicks() - then);

    fps++;
    if (SDL_GetTicks() >= nextFPS) {
      game->stats.fps = fps;
		  fps = 0;
		  nextFPS = SDL_GetTicks() + 1000;
	  }

    lockFrameRate(&then, &remainder, game->settings.fpsLock);
  }
}

struct Djinni_Game_RunnerStruct Djinni_Game_Runner = {
  .execute = execute
};