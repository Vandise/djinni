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
  TILE_LAYER_TYPE
} DJINNI_LAYER_TYPE;

typedef struct Djinni_Map_TileStruct {
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

typedef struct Djinni_Map_ObjectStruct {
  int x;
  int y;
  int sx;
  int sy;
  AtlasImage* img;
} MapObject;

typedef struct Djinni_WorldMapLayerStruct {
  int id;
  DJINNI_LAYER_TYPE type;

  int nObjects;
  DjinniArray* atlases;
  MapObject* objects;

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
} WorldMap;

struct Djinni_MapStruct {
  WorldMap* (*create)();
  void (*load)(WorldMap*, Renderer*);
  void (*draw)(WorldMap*, Renderer*, Camera*, double);
  void (*setMapDataFile)(WorldMap*, char*);

  void (*inspect)(WorldMap*);
  void (*destroy)(WorldMap*);
};

extern struct Djinni_MapStruct Djinni_Map;

#endif