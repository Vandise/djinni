#include <stdlib.h>
#include "djinni/game/stage.h"
#include "djinni/util/array.h"

static DjinniArray* stages = NULL;
static int active_stage = -1;

inline void djinni_game_stage_initialize(int n_stages) {
  stages = djinni_array_initialize(n_stages);
}

Djinni_GameStage* djinni_game_stage_create(
    void (*onCreate)(Djinni_GameStage* stage),
    void (*prepare)(Djinni_GameStage* stage),
    void (*update)(Djinni_GameStage* stage, double dt),
    void (*draw)(Djinni_GameStage* stage, double dt),
    void (*onDestroy)(Djinni_GameStage* stage)
) {
  Djinni_GameStage* s = malloc(sizeof(Djinni_GameStage));
    s->id = stages->used;
    s->ready = 0;
    s->onCreate = onCreate;
    s->prepare = prepare;
    s->update = update;
    s->draw = draw;
    s->onDestroy = onDestroy;

  djinni_array_insert(stages, s);

  return s;
}

inline void djinni_game_stage_set_active_stage(int id) {
  active_stage = id;
}

inline Djinni_GameStage* djinni_game_stage_get_active_stage() {
  return stages->data[active_stage];
}

void djinni_game_stage_change_stage(int id) {
  Djinni_GameStage* current_stage = NULL;
  Djinni_GameStage* next_stage = stages->data[id];

  if (active_stage >= 0) {
    Djinni_GameStage* current_stage = stages->data[active_stage];
    current_stage->onDestroy(current_stage);
  }

  next_stage->onCreate(next_stage);

  djinni_game_stage_set_active_stage(id);
}

void djinni_game_stage_destroy_stage(Djinni_GameStage* stage) {
  free(stage);
}

void djinni_game_stage_destroy() {
  if (stages != NULL) {
    djinni_array_destroy(stages, free);
  }
}