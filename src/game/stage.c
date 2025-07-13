#include "djinni/util/util.h"
#include "djinni/game/stage.h"

static Stage* create(
  int id,
  void (*onCreate)(Stage*, Game*, Stage*),
  void (*prepare)(Stage*, Game*),
  void (*update)(Stage*, Game*, double),
  void (*draw)(Stage*, Game*, double),
  void (*onDestroy)(Stage*, Game*, Stage*)
) {
  Djinni_Util_Logger.log_dev("Djinni::Game::Stage.create( id:(%d) )", id);

  Stage* s = malloc(sizeof(Stage));
  s->id = id;
  s->ready = 0;
  s->onCreate = onCreate;
  s->prepare = prepare;
  s->update = update;
  s->draw = draw;
  s->onDestroy = onDestroy;

  return s;
}

static void destroy(Stage* s) {
  Djinni_Util_Logger.log_dev("Djinni::Game::Stage.destroy( id:(%d) )", s->id);

  free(s);
}

static void arrayDestroyCallback(void* stage) {
  destroy(stage);
}

struct Djinni_StageStruct Djinni_Stage = {
  .create = create,
  .destroy = destroy,
  .arrayDestroyCallback = arrayDestroyCallback
};