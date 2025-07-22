#ifndef DJINNI_GAME_WORLD
#define DJINNI_GAME_WORLD 1

#include "djinni/game/shared.h"
#include "djinni/geometry/shared.h"
#include "djinni/renderable/shared.h"

#include "djinni/util/array.h"
#include "djinni/game/camera.h"
#include "djinni/renderable/entity.h"
#include "djinni/geometry/grid.h"

typedef struct Djinni_Game_World_SettingsStruct {
  int width;
  int height;

  int finestGridSize;
  int mediumGridSize;
  int coarseGridSize;
  int gridCellCapacity;
} WorldSettings;

/*
  todo:
    revisit entities once isometric processing begins
    entities will likely need to be a binary tree sorted by the X-Y coordinate
    for rendering precedence
*/
typedef struct Djinni_Game_WorldStruct {
  DjinniArray* entities;
  Grid* grid;
  Camera* camera;

  WorldSettings settings;
} World;

struct Djinni_Game_GameWorldStruct {
  World* (*create)(WorldSettings);
  void (*addEntity)(World*, Entity*);
  void (*removeEntity)(Game*, Entity*, double);
  void (*update)(Game*, ViewportBounds, DJINNI_RING, double);
  void (*destroy)(World*);
};

extern struct Djinni_Game_GameWorldStruct Djinni_World;

#endif