#ifndef DJINNI_MAP
#define DJINNI_MAP 1

#include <stdlib.h>
#include "djinni/map/shared.h"
#include "djinni/game/shared.h"
#include "djinni/util/array.h"
#include "djinni/video/texture.h"

#define DJINNI_MAX_MAP_LAYERS 10

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
  int tileId;
} MapTile;

typedef struct Djinni_Map_ObjectStruct {
  int x;
  int y;
  int sx;
  int sy;
  Texture* texture;
} MapObject;

typedef struct Djinni_WorldMapLayerStruct {
  int id;
  DJINNI_LAYER_TYPE type;

  int nObjects;
  DjinniArray* textures;
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

  int width;
  int height;
  int tileWidth;
  int tileHeight;

  int nXTiles;
  int nYTiles;
  int nObjects;

  DjinniArray* tiles;
  MapTile* data;
  MapObject* objects;

  WorldMapLayer layers[DJINNI_MAX_MAP_LAYERS];
} WorldMap;

struct Djinni_MapStruct {
  WorldMap* (*create)(int, int, int, int, DJINNI_MAP_TYPE);
  void (*load)(WorldMap*, Renderer*);
  void (*draw)(WorldMap*, Renderer*, Camera*, double);

  void (*addTexture)(WorldMap*, Texture*, DJINNI_MAP_LAYER);
  void (*addTile)(WorldMap*, int, DJINNI_MAP_LAYER);

  void (*inspect)(WorldMap*);
  void (*destroy)(WorldMap*);
};

extern struct Djinni_MapStruct Djinni_Map;

#endif