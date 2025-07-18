#include "djinni/util/util.h"
#include "djinni/djinni.h"

/*
  Todo:
    entities will need to be sorted. Likely to a binary tree, to support Painter's Algorithm
    for isometric
*/

static void prepare(Game* game) {
  Djinni.Video->Renderer->setDrawColor(Djinni.renderer, Djinni.renderer->backgroundColor);
  Djinni.Video->Renderer->clear(Djinni.renderer);

  game->activeStage->prepare(game->activeStage, game);
}

static void update(Game* game, double dt) {
  Djinni.Game->Camera->update(game->camera, dt);

  if (game->camera->update != NULL) {
    game->camera->update(game->camera, game, dt);
  }

  ViewportBounds bounds = Djinni.Game->Camera->getViewportBounds(game->camera);

  //
  // tick fine ring every frame
  //
  // this will check for grid promotions/demotions, fine physics, and advanced logic 
  //
  Djinni.Physics->tick(game, dt, DJINNI_RING_FINE);

  Djinni.Game->World->update(
    game,
    bounds,
    DJINNI_RING_FINE,
    dt
  );

  //
  // tick medium ring tick to the locked frame rate
  //      example: settings.mediumRingTick = 0.1
  //      60fps = every 6 frames
  //
  //  This should check for grid promotions/demotions, simple physics, and general logic
  //
  if ( (dt - game->stats.mediumdt) > game->settings.mediumRingTick ) {
    game->stats.mediumdt = dt;

    Djinni.Physics->tick(game, dt, DJINNI_RING_MEDIUM);

    Djinni.Game->World->update(
      game,
      bounds,
      DJINNI_RING_MEDIUM,
      dt
    );
  }

  //
  // tick coarse ring tick to the locked frame rate
  //      example: settings.mediumRingTick = 0.9
  //      60fps = every 54 frames
  //
  //  this should only check grid promotions/demotions and not execute any physics
  //
  if ( (dt - game->stats.coarsedt) > game->settings.coarseRingTick ) {
    game->stats.coarsedt = dt;

    Djinni.Game->World->update(
      game,
      bounds,
      DJINNI_RING_COARSE,
      dt
    );
  }

  game->activeStage->update(game->activeStage, game, dt);

  game->stats.dt = dt;
}

static void draw(Game* game, double dt) {
  Djinni.Renderable->draw(Djinni.renderer, game->world->grid, game->camera, DJINNI_RING_FINE);

  game->activeStage->draw(game->activeStage, game, dt);
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
      update(game, then);
  		dt = (tmpDelta - 1);
  	}

    update(game, then);

    draw(game, then);
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