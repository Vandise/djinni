#ifndef DJINNI_GAME_STAGE
#define DJINNI_GAME_STAGE 1

#include "djinni/game/shared.h"

typedef struct Djinni_Game_StageStruct {
  int id;
  int ready;

  void (*onCreate)(Stage*, Game*, Stage*);
  void (*prepare)(Stage*, Game*);
  void (*update)(Stage*, Game*, double);
  void (*draw)(Stage*, Game*, double dt);
  void (*onDestroy)(Stage*, Game*, Stage*);
} Stage;

struct Djinni_StageStruct {
  Stage* (*create)(
    int id,
    void (*onCreate)(Stage*, Game*, Stage*),
    void (*prepare)(Stage*, Game*),
    void (*update)(Stage*, Game*, double),
    void (*draw)(Stage*, Game*, double),
    void (*onDestroy)(Stage*, Game*, Stage*)
  );

  void (*destroy)(Stage*);
  void (*arrayDestroyCallback)(void*);
};

extern struct Djinni_StageStruct Djinni_Stage;

#endif