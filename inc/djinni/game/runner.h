#ifndef DJINNI_GAME_RUNNER
#define DJINNI_GAME_RUNNER 1

#include "djinni/game/shared.h"

struct Djinni_Game_RunnerStruct {
  void (*execute)(Game*);
};

extern struct Djinni_Game_RunnerStruct Djinni_Game_Runner;

#endif