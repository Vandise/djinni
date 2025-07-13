#ifndef DJINNI_GAME_CAMERA
#define DJINNI_GAME_CAMERA 1

#include "djinni/game/shared.h"
#include "djinni/geometry/coordinate.h"

typedef struct Djinni_Game_CameraStruct {
  int renderWidth;
  int renderHeight;

  Point point;
} Camera;

struct Djinni_Game_GameCameraStruct {
  Camera* (*create)(int, int, int, int);
  int (*inViewport)(Camera*, Point);
  void (*inspect)(Camera*);
  void (*destroy)(Camera*);
};

extern struct Djinni_Game_GameCameraStruct Djinni_Camera;

#endif