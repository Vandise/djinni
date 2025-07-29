#ifndef DJINNI_GAME_STAGE
#define DJINNI_GAME_STAGE 1

#include "djinni/game/shared.h"

typedef struct Djinni_Game_StageStruct {
  int ready;
  int id;

  void (*onCreate)(Djinni_GameStage* stage);
  void (*prepare)(Djinni_GameStage* stage);
  void (*update)(Djinni_GameStage* stage, double dt);
  void (*draw)(Djinni_GameStage* stage, double dt);
  void (*onDestroy)(Djinni_GameStage* stage);
} Djinni_GameStage;

void djinni_game_stage_initialize(int n_stages);

Djinni_GameStage* djinni_game_stage_create(
  void (*onCreate)(Djinni_GameStage* stage),
  void (*prepare)(Djinni_GameStage* stage),
  void (*update)(Djinni_GameStage* stage, double dt),
  void (*draw)(Djinni_GameStage* stage, double dt),
  void (*onDestroy)(Djinni_GameStage* stage)
);

void djinni_game_stage_set_active_stage(int id);
Djinni_GameStage* djinni_game_stage_get_active_stage();
void djinni_game_stage_change_stage(int id);
void djinni_game_stage_destroy_stage(Djinni_GameStage* stage);
void djinni_game_stage_destroy();

#endif