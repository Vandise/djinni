#include <stdlib.h>
#include "djinni/game/game.h"

static Djinni_Game* game = NULL;

void djinni_game_initialize(Djinni_GameSettings settings) {
  game = malloc(sizeof(Djinni_Game));
    game->paused = 0;
    game->terminated = 0;
    game->settings = settings;
}

inline Djinni_Game* djinni_game_get_game() {
  return game;
}

inline int djinni_game_paused() {
  return game->paused;
}

inline void djinni_game_pause() {
  game->paused = 1;
}

inline void djinni_game_resume() {
  game->paused = 0;
}

inline int djinni_game_terminated() {
  return game->terminated;
}

inline void djinni_game_terminate() {
  game->terminated = 1;
}

void djinni_game_destroy() {
  free(game);
}