#ifndef DJINNI_GAME_CAMERA
#define DJINNI_GAME_CAMERA 1

#include "djinni/common.h"
#include "djinni/ecs/shared.h"
#include "djinni/game/shared.h"

typedef struct Djinni_Game_CameraStruct {
  int x;
  int y;
  int width;
  int height;

  float zoom;

  struct {
    DjinniEntityId id;
    int x_offset;
    int y_offset; 
  } follow;
} Djinni_Game_Camera;

void djinni_game_camera_initialize(Djinni_GameSettings settings);
void djinni_game_camera_update();
void djinni_game_camera_follow(DjinniEntityId id, int x_offset, int y_offset);
int djinni_game_camera_entity_in_viewport(DjinniEntityId id);
Djinni_Game_Camera* djinni_game_camera_get_camera();
void djinni_game_camera_destroy();

#endif