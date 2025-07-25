#include "djinni/util/util.h"
#include "djinni/game/game.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Game.initialize");

  Djinni_Game.Stage = &Djinni_Stage;
  Djinni_Game.Runner = &Djinni_Game_Runner;
  Djinni_Game.Input = &Djinni_Game_Input;
  Djinni_Game.World = &Djinni_World;
  Djinni_Game.Camera = &Djinni_Camera;
}

static Game* create(WorldSettings ws) {
  Game* g = malloc(sizeof(Game));
  g->paused = 0;
  g->terminated = 0;
  g->inputEnabled = 0;

  g->settings.fpsLock = 60.0;
  g->settings.logicRate = (g->settings.fpsLock / 1000);
  g->settings.mediumRingTick = 0.1;
  g->settings.coarseRingTick = 0.9;

  g->activeStage = NULL;
  g->stages = Djinni_Util_Array.initialize(10);
  g->world = Djinni_World.create(ws);
  g->camera = NULL;
  g->stats.dt = g->stats.mediumdt = g->stats.coarsedt = 0.0;

  for (int i = 0; i < MAX_IO_KEY_CODE; i++) {
    g->keyboard[i] = 0;
  }

  return g;
}

static int addStage(Game* game, Stage* stage) {
  int index = game->stages->used;

  Djinni_Util_Array.insert(game->stages, stage);

  return index;
}

void setCamera(Game* game, Camera* camera) {
  game->camera = camera;
  game->world->camera = camera;
}

static void changeStage(Game* game, int id) {
  Stage* stage = NULL;

  for (int i = 0; i < game->stages->used; i++) {
    stage = (Stage*)(game->stages->data[i]);
    if (stage->id == id) {
      break;
    } else {
      stage = NULL;
    }
  }

  if (stage == NULL) {
    Djinni_Util_Logger.log_warn("Djinni::Game.changeStage( id:(%d) ) - stage not found", id);
    return;
  }

  if (game->activeStage == NULL) {
    game->activeStage = stage;
    game->activeStage->onCreate(stage, game, NULL);
    return;
  }

  if (game->activeStage->id == stage->id) {
    Djinni_Util_Logger.log_warn("Djinni::Game.changeStage( id:(%d) ) - currently active", id);
    return;
  }

  game->activeStage->onDestroy(game->activeStage, game, stage);
  game->activeStage->onCreate(stage, game, game->activeStage);
  game->activeStage = stage;
}

static void pause(Game* game) {
  game->paused = 1;
}

static void resume(Game* game) {
  game->paused = 0;
}

static void enableInput(Game* game) {
  game->inputEnabled = 1;
}

static void disableInput(Game* game) {
  game->inputEnabled = 0;
}

static void terminate(Game* game) {
  game->terminated = 1;
}

static void destroy(Game* game) {
  Djinni_Util_Logger.log_dev("Djinni::Game.destroy( address:(%p) )", game);

  Djinni_Util_Array.destroy(
    game->stages,
    Djinni_Stage.arrayDestroyCallback
  );

  Djinni_World.destroy(game->world);
  Djinni_Camera.destroy(game->camera);

  free(game);
}

struct Djinni_GameStruct Djinni_Game = {
  .initialize = initialize,

  .pause = pause,
  .resume = resume,
  .enableInput = enableInput,
  .disableInput = disableInput,
  .terminate = terminate,

  .create = create,
  .addStage = addStage,
  .changeStage = changeStage,

  .setCamera = setCamera,

  .destroy = destroy
};