#ifndef DJINNI_GAME
#define DJINNI_GAME 1

#include "djinni/util/array.h"
#include "djinni/game/shared.h"
#include "djinni/game/stage.h"

typedef struct Djinni_Game_SettingsStruct {
  float fpsLock;
  float logicRate;
} Djinni_Game_Settings;

typedef struct Djinni_Game_GameStruct {
  int paused;
  int terminated;
  Djinni_Game_Settings settings;

  Stage* activeStage;

  DjinniArray* stages;
} Game;

struct Djinni_GameStruct {
  struct Djinni_StageStruct* Stage;

  void (*initialize)();

  Game* (*create)();
  int (*addStage)(Game*, Stage*);
  void (*changeStage)(Game*, int);
  void (*destroy)(Game*);
};

extern struct Djinni_GameStruct Djinni_Game;

#endif