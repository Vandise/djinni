#ifndef DJINNI_GAME_CAMERA
#define DJINNI_GAME_CAMERA 1

#include "djinni/game/shared.h"
#include "djinni/renderable/entity.h"
#include "djinni/geometry/coordinate.h"

typedef struct ViewportBoundsStruct {
  int x1;
  int y1;
  int x2;
  int y2;
} ViewportBounds;

typedef struct Djinni_Game_CameraSettingsStruct {
  int fixed;
  int xOffset;
  int yOffset;

  Entity* following;
} CameraSettings;

typedef struct Djinni_Game_CameraStruct {
  float zoom;
  int screenWidth;
  int screenHeight;

  CameraSettings settings;

  Point point;

  void (*update)(Camera*, Game*, double);
} Camera;

struct Djinni_Game_GameCameraStruct {
  Camera* (*create)(int, int, int, int);
  void (*setDrawPoint)(Camera*, Point);
  int (*inViewport)(Camera*, Point);
  void (*follow)(Camera*, Entity*, int, int);
  Point (*entityCoordinateToScreen)(Camera*, Entity*);
  Point (*coordinateToScreen)(Camera*, Coordinate);
  ViewportBounds (*getViewportBounds)(Camera*);
  void (*update)(Camera*, double);
  void (*inspect)(Camera*);
  void (*destroy)(Camera*);
};

extern struct Djinni_Game_GameCameraStruct Djinni_Camera;

#endif