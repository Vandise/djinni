#ifndef DJINNI_GAME
#define DJINNI_GAME 1

#include "djinni/game/shared.h"
#include "djinni/game/stage.h"
#include "djinni/game/input.h"

typedef struct Djinni_Game_SettingsStruct {
  int n_stages;
  int n_entities;

  float fps_lock;
  float logic_rate;

  float medium_ring_tick;
  float coarse_ring_tick;
} Djinni_GameSettings;

typedef struct Djinni_Game_GameStruct {
  int paused;
  int terminated;

  Djinni_GameSettings settings;
} Djinni_Game;

void djinni_game_initialize(Djinni_GameSettings settings);
int djinni_game_paused();
void djinni_game_pause();
void djinni_game_resume();
int djinni_game_terminated();
void djinni_game_terminate();
void djinni_game_destroy();

#endif