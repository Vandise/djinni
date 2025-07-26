#ifndef DJINNI_MAP
#define DJINNI_MAP 1

#include <stdlib.h>
#include "djinni/map/shared.h"
#include "djinni/game/shared.h"
#include "djinni/util/shared.h"
#include "djinni/util/array.h"
#include "djinni/renderable/shared.h"
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
  ISOMETRIC_TILE_TYPE,
  ISOMETRIC_ENTITY_TYPE
} DJINNI_ISOMETRIC_OBJECT_TYPE;

/*
  tiles are an array of objects with an index and atlas coordinates
  tiles for a specific coordinate may also be empty with a 0

  tiles: Array(Object)
    Object:
      index(int): spritesheet atlas index
      atlas(int): index of the spritesheet atlas in the layer

  Example:
    "tiles":[{"index":2,"atlas":0},0]
*/
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

/*
  An object for containing map object data for the developer to
  create entities, objects, and triggers in the objectLoader function

  objects: Array(WorldMapObject)
    id(int): meta data for the developer to set specific functionality
    type(int): the type of object to create
    x(int): relative X position of the entity ( absolute 0 = top-left )
    y(int): relative Y position of the entity ( absolute 0 = top-left )

    index(int): spritesheet atlas index
    atlas(int): index of the spritesheet atlas in the layer
      
  Example:
    "objects":[
      {
        "id": 0,
        "type": 0,
        "x": 0,
        "y": 0,
        "atlas": 0,
        "index": 24
      }
    ]
*/
typedef struct Djinni_WorldMapObjectStruct {
  int id;
  int type;

  int layer;
  int x;
  int y;
  int atlas;
  int atlasIndex;

  char textureFile[DJINNI_MAX_MAP_FILENAME];
} WorldMapObject;

/*

  Isometric reference object for quick sorting and handling of drawing

*/
typedef struct Djinni_WorldMapIsometricObjectStruct {
  DJINNI_ISOMETRIC_OBJECT_TYPE type;

  int* layer;
  int* x;
  int* y;
  int* width;
  int* height;

  MapTile* tile;
  Entity*  entity;
} IsometricObject;

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
  int baseTileWidth;
  int baseTileHeight;

  WorldMapLayer layers[DJINNI_MAX_MAP_LAYERS];

  DjinniArray* isometricObjects;

  void (*objectLoader)(Game*, WorldMapObject*);
} WorldMap;

struct Djinni_MapStruct {
  WorldMap* (*create)(int, int);
  void (*load)(Game*, WorldMap*, Renderer*);
  void (*setMapDataFile)(WorldMap*, char*);

  void (*setObjectLoader)(WorldMap*, void (*objectLoader)(Game*, WorldMapObject*));

  void (*inspect)(WorldMap*);
  void (*destroy)(WorldMap*);
};

extern struct Djinni_MapStruct Djinni_Map;

#endif