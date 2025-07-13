#include "djinni/util/util.h"
#include "djinni/game/game.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Game.initialize");

  Djinni_Game.Stage = &Djinni_Stage;
}

static Game* create() {
  Game* g = malloc(sizeof(Game));
  g->paused = 0;
  g->terminated = 0;

  g->settings.fpsLock = 60.0;
  g->settings.logicRate = (g->settings.fpsLock / 1000);

  g->activeStage = NULL;
  g->stages = Djinni_Util_Array.initialize(10);

  return g;
}

static int addStage(Game* game, Stage* stage) {
  int index = game->stages->used;

  Djinni_Util_Array.insert(game->stages, stage);

  return index;
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

static void destroy(Game* game) {
  Djinni_Util_Logger.log_dev("Djinni::Game.destroy( address:(%p) )", game);

  Djinni_Util_Array.destroy(
    game->stages,
    Djinni_Stage.arrayDestroyCallback
  );

  free(game);
}

struct Djinni_GameStruct Djinni_Game = {
  .initialize = initialize,

  .create = create,
  .addStage = addStage,
  .changeStage = changeStage,
  .destroy = destroy
};