#ifndef DJINNI_MAP
#define DJINNI_MAP 1

#include <stdlib.h>
#include "djinni/map/shared.h"
#include "djinni/util/array.h"
#include "djinni/video/texture.h"

typedef enum {
  STANDARD_MAP_TYPE,
  ISOMETRIC_MAP_TYPE
} DJINNI_MAP_TYPE;

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
} WorldMap;

struct Djinni_MapStruct {
  WorldMap* (*create)(int, int, int, int, DJINNI_MAP_TYPE);
  void (*load)(WorldMap*, Renderer*);
  void (*draw)(WorldMap*, Renderer*, double);
  void (*inspect)(WorldMap*);
  void (*destroy)(WorldMap*);
};

extern struct Djinni_MapStruct Djinni_Map;

#endif