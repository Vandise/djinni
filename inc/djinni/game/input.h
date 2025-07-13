#ifndef DJINNI_GAME_IO
#define DJINNI_GAME_IO 1

#include "djinni/game/shared.h"

struct Djinni_Game_InputStruct {
  void (*process)(Game*);
};

extern struct Djinni_Game_InputStruct Djinni_Game_Input;

#endif