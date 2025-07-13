#ifndef DJINNI_GAME_WORLD
#define DJINNI_GAME_WORLD 1

#include "djinni/game/shared.h"
#include "djinni/renderable/shared.h"

#include "djinni/util/array.h"
#include "djinni/renderable/entity.h"

typedef struct Djinni_Game_WorldStruct {
  DjinniArray* entities;
} World;

struct Djinni_Game_GameWorldStruct {
  World* (*create)();
  void (*addEntity)(World*, Entity*);
  void (*removeEntity)(World*, Entity*);
  void (*destroy)(World*);
};

extern struct Djinni_Game_GameWorldStruct Djinni_World;

#endif