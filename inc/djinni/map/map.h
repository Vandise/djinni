#ifndef DJINNI_MAP
#define DJINNI_MAP 1

#include <stdlib.h>
#include "djinni/map/shared.h"
#include "djinni/game/shared.h"
#include "djinni/util/shared.h"
#include "djinni/util/array.h"
#include "djinni/video/imageAtlas.h"

#define DJINNI_MAX_MAP_LAYERS 10
#define DJINNI_MAX_MAP_FILENAME 255

typedef enum {
  STANDARD_MAP_TYPE,
  ISOMETRIC_MAP_TYPE
} DJINNI_MAP_TYPE;

typedef enum {
  BACKGROUND_LAYER,

  GROUND_LAYER,
  GROUND_ANIMATION_LAYER,

  OBJECT_BASE_LAYER,
  OBJECT_DETAIL_LAYER,
  OBJECT_ANIMATION_LAYER,

  ENTITY_LAYER,

  OCCLUSION_LAYER,
  OCCLUSION_ANIMATION_LAYER,

  OVERLAY_LAYER
} DJINNI_MAP_LAYER;

typedef enum {
  UNINITIALIZED_LAYER_TYPE,
  TILE_LAYER_TYPE,
  ENTITY_LAYER_TYPE,
  OBJECT_LAYER_TYPE
} DJINNI_LAYER_TYPE;

typedef enum {
  PLAYER_ENTITY_MAP_OBJECT
} DJINNI_MAP_OBJECT_TYPE;

typedef struct Djinni_Map_TileStruct {
  int empty;

  int layer;
  int tileIndex;
  int atlasIndex;

  // map coords
  int x;
  int y;

  // screen coords
  int sx;
  int sy;

  // data
  int width;
  int height;
} MapTile;

typedef struct Djinni_WorldMapObjectStruct {
  int id;
  int type;

  int x;
  int y;
  int atlas;
  int atlasIndex;

  char textureFile[DJINNI_MAX_MAP_FILENAME];
} WorldMapObject;

typedef struct Djinni_WorldMapLayerStruct {
  int id;
  DJINNI_LAYER_TYPE type;

  DjinniArray* atlases;
  DjinniArray* objects;

  struct Djinni_MapLayerTilesStruct {
    int nTiles;
    int nxTiles;
    int nyTiles;
    int tileWidth;
    int tileHeight;
    MapTile* data;
  } tiles;
} WorldMapLayer;

typedef struct Djinni_WorldMapStruct {
  DJINNI_MAP_TYPE type;

  char mapFileName[DJINNI_MAX_MAP_FILENAME];

  int width;
  int height;

  WorldMapLayer layers[DJINNI_MAX_MAP_LAYERS];

  void (*objectLoader)(World*, WorldMapObject*, DJINNI_MAP_LAYER);
} WorldMap;

struct Djinni_MapStruct {
  WorldMap* (*create)();
  void (*load)(WorldMap*, Renderer*);
  void (*setMapDataFile)(WorldMap*, char*);

  void (*setObjectLoader)(WorldMap*, void (*objectLoader)(World*, WorldMapObject*, DJINNI_MAP_LAYER));

  void (*inspect)(WorldMap*);
  void (*destroy)(WorldMap*);
};

extern struct Djinni_MapStruct Djinni_Map;

#endif