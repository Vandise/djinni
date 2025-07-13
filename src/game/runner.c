#include "djinni/util/util.h"
#include "djinni/djinni.h"

static void execute(Game* game) {
  Djinni_Util_Logger.log_debug("Djinni::Game::Runner.execute( game:(%p) )", game);
}

struct Djinni_Game_RunnerStruct Djinni_Game_Runner = {
  .execute = execute
};