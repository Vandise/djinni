#ifndef DJINNI_GAME_WORLD
#define DJINNI_GAME_WORLD 1

#include "djinni/game/shared.h"
#include "djinni/geometry/shared.h"
#include "djinni/renderable/shared.h"
#include "djinni/map/shared.h"

#include "djinni/util/array.h"
#include "djinni/game/camera.h"
#include "djinni/renderable/entity.h"
#include "djinni/geometry/grid.h"
#include "djinni/map/map.h"

typedef enum {
  NO_GRID_WORLD_TYPE,
  STANDARD_WORLD_TYPE,
  MAP_WORLD_TYPE
} DJINNI_WORLD_TYPE;

typedef struct Djinni_Game_World_SettingsStruct {
  DJINNI_WORLD_TYPE type;

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
  WorldMap* worldMap;

  WorldSettings settings;
} World;

struct Djinni_Game_GameWorldStruct {
  World* (*create)(WorldSettings);
  void (*addEntity)(World*, Entity*);
  void (*removeEntity)(Game*, Entity*, double);
  void (*setWorldMap)(World*, WorldMap*);
  void (*update)(Game*, ViewportBounds, DJINNI_RING, double);
  void (*draw)(Renderer*, Game*, double);
  void (*destroy)(World*);
};

extern struct Djinni_Game_GameWorldStruct Djinni_World;

#endif