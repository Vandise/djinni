#ifndef DJINNI_GAME
#define DJINNI_GAME 1

#include "djinni/game/shared.h"
#include "djinni/game/stage.h"
#include "djinni/game/input.h"
#include "djinni/game/runner.h"

typedef struct Djinni_Game_SettingsStruct {
  struct {
    int n_stages;
    int n_entities;
  } pool_settings;

  struct {
    float fps_lock;
    float logic_rate;
  } engine_settings;

  struct {
    int cell_capacity;
    int finest_size;
    int medium_size;
    int coarse_size;
    float medium_ring_tick;
    float coarse_ring_tick;
  } grid_settings;

  struct {
    int x;
    int y;
    int width;
    int height;
    float zoom;
  } camera_settings;

} Djinni_GameSettings;

typedef struct Djinni_Game_GameStruct {
  int paused;
  int terminated;

  Djinni_GameSettings settings;
} Djinni_Game;

void djinni_game_initialize(Djinni_GameSettings settings);
Djinni_Game* djinni_game_get_game();
int djinni_game_paused();
void djinni_game_pause();
void djinni_game_resume();
int djinni_game_terminated();
void djinni_game_terminate();
void djinni_game_destroy();

#endif