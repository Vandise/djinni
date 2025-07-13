#include "djinni/util/util.h"
#include "djinni/djinni.h"

static void prepare(Game* game) {

  game->activeStage->prepare(game->activeStage, game);

  Djinni.Video->Renderer->setDrawColor(Djinni.renderer, Djinni.renderer->backgroundColor);
  Djinni.Video->Renderer->clear(Djinni.renderer);
}

static void present(Game* game) {
  Djinni.Video->Renderer->present(Djinni.renderer);
}

static void execute(Game* game) {
  Djinni_Util_Logger.log_debug("Djinni::Game::Runner.execute( game:(%p) )", game);

  /*
    1. prepare (bg color, clear)
    2. User IO
    3. Game logic
        Update, Check for collisions
    4. Draw (stage assets, entities)
    5. Present to window
  */

  //while(game->terminated == 0) {
    prepare(game);
    Djinni.Game->Input->process(game);
    present(game);
  //}
}

struct Djinni_Game_RunnerStruct Djinni_Game_Runner = {
  .execute = execute
};