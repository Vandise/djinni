#ifndef DJINNI_GAME
#define DJINNI_GAME 1

#include "djinni/util/array.h"
#include "djinni/game/shared.h"
#include "djinni/game/stage.h"
#include "djinni/game/runner.h"
#include "djinni/game/input.h"
#include "djinni/game/world.h"
#include "djinni/game/camera.h"

typedef struct Djinni_Game_SettingsStruct {
  float fpsLock;
  float logicRate;
} Djinni_Game_Settings;

typedef struct Djinni_Game_StatsStruct {
  int fps;
} Djinni_Game_Stats;

typedef struct Djinni_Game_GameStruct {
  int paused;
  int terminated;
  int inputEnabled;
  int keyboard[MAX_IO_KEY_CODE];

  Djinni_Game_Settings settings;
  Djinni_Game_Stats stats;

  World* world;
  Stage* activeStage;
  Camera* camera;

  DjinniArray* stages;
} Game;

struct Djinni_GameStruct {
  struct Djinni_Game_RunnerStruct* Runner;
  struct Djinni_StageStruct* Stage;
  struct Djinni_Game_InputStruct* Input;
  struct Djinni_Game_GameWorldStruct* World;
  struct Djinni_Game_GameCameraStruct* Camera;

  void (*initialize)();

  Game* (*create)();

  void (*pause)(Game* game);
  void (*resume)(Game* game);
  void (*enableInput)(Game* game);
  void (*disableInput)(Game* game);
  void (*terminate)(Game* game);

  int (*addStage)(Game*, Stage*);
  void (*changeStage)(Game*, int);

  void (*setCamera)(Game*, Camera*);

  void (*destroy)(Game*);
};

extern struct Djinni_GameStruct Djinni_Game;

#endif