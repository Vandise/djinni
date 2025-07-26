#include "djinni/util/util.h"
#include "djinni/map/map.h"
#include "djinni/game/camera.h"
#include "djinni/geometry/isometric.h"
#include "djinni/util/json.h"

static int getIndex(int x, int y, int maxX) {
  return y * maxX + x;
}

static WorldMap* create() {
  WorldMap* m = malloc(sizeof(WorldMap));

  m->mapFileName[0] = 0;
  m->isometricObjects = NULL;

  for (int i = 0; i < DJINNI_MAX_MAP_LAYERS; i++) {
    WorldMapLayer* layer = &(m->layers[i]);
    layer->id = i;
    layer->type = UNINITIALIZED_LAYER_TYPE;
    layer->atlases = NULL;
    layer->objects = NULL;
    layer->tiles.nTiles = -1;
    layer->tiles.tileWidth = -1;
    layer->tiles.tileHeight = -1;
    layer->tiles.data = NULL;
  }

  return m;
}

static void loadLayer(WorldMap* m, Renderer* r, cJSON* layerNode) {
  int layerId = cJSON_GetObjectItem(layerNode, "layer")->valueint;
  WorldMapLayer* mapLayer = &(m->layers[layerId]);
  mapLayer->type = cJSON_GetObjectItem(layerNode, "type")->valueint;

  //
  // atlases
  //
  cJSON* atlasesNode = cJSON_GetObjectItem(layerNode, "atlases");
  int nAtlases = cJSON_GetArraySize(atlasesNode);

  if (nAtlases && mapLayer->atlases == NULL) {
    mapLayer->atlases = Djinni_Util_Array.initialize(4);
  }

  for (int i = 0; i < nAtlases; i++) {
    cJSON* atlasJSON = cJSON_GetArrayItem(atlasesNode, i);

    ImageAtlas* atlas = Djinni_Video_Image_Atlas.load(
      r,
      cJSON_GetObjectItem(atlasJSON, "image")->valuestring,
      cJSON_GetObjectItem(atlasJSON, "data")->valuestring
    );

    Djinni_Util_Array.insert(mapLayer->atlases, atlas);
  }

  //
  // tiles
  //
  if (mapLayer->type == TILE_LAYER_TYPE) {
    mapLayer->tiles.tileWidth = cJSON_GetObjectItem(layerNode, "tileWidth")->valueint;
    mapLayer->tiles.tileHeight = cJSON_GetObjectItem(layerNode, "tileHeight")->valueint;
    mapLayer->tiles.nxTiles = m->width / mapLayer->tiles.tileWidth;
    mapLayer->tiles.nyTiles = m->height / mapLayer->tiles.tileHeight;
    mapLayer->tiles.data = malloc(sizeof(MapTile) * mapLayer->tiles.nxTiles * mapLayer->tiles.nyTiles);

    cJSON* tilesNode = cJSON_GetObjectItem(layerNode, "tiles");
    mapLayer->tiles.nTiles = cJSON_GetArraySize(tilesNode);

    for (int i = 0; i < mapLayer->tiles.nTiles; i++) {
      cJSON* tileNode = cJSON_GetArrayItem(tilesNode, i);
      MapTile* mt = &(mapLayer->tiles.data[i]);

      // skip blank tiles
      if (tileNode->type != cJSON_Object) {
        mt->empty = 1;
        continue;
      }

      mt->layer = layerId;
      mt->empty = 0;
      mt->tileIndex = cJSON_GetObjectItem(tileNode, "index")->valueint;
      mt->atlasIndex = cJSON_GetObjectItem(tileNode, "atlas")->valueint;
      mt->width = mapLayer->tiles.tileWidth;
      mt->height = mapLayer->tiles.tileHeight;

      mt->x = i % mapLayer->tiles.nxTiles;
      mt->y = i / mapLayer->tiles.nxTiles;

      if (m->type == ISOMETRIC_MAP_TYPE) {
        Coordinate isoxy = Djinni_Geometry_Isometric.xytoiso(
          m->height, mt->x, mt->y, mapLayer->tiles.tileWidth, mapLayer->tiles.tileHeight
        );

        mt->sx = isoxy.x;
        mt->sy = isoxy.y;

        mt->x = mt->sx;
        mt->y = mt->sy;

        IsometricObject* iso = malloc(sizeof(IsometricObject));
          iso->type = ISOMETRIC_TILE_TYPE;
          iso->layer = &(mt->layer);
          iso->x = &(mt->x);
          iso->y = &(mt->y);
          iso->width = &(mt->width);
          iso->height = &(mt->height);
          iso->tile = mt;

        Djinni_Util_Array.insert(m->isometricObjects, iso);

      } else {
        mt->sx = mt->x;
        mt->sy = mt->y;
      }
    }
  }

  //
  // Objects
  //
  /*
  cJSON* objectsNode = cJSON_GetObjectItem(layerNode, "objects");
  if (objectsNode != NULL) {
    int nObjects = cJSON_GetArraySize(objectsNode);

    mapLayer->objects = Djinni_Util_Array.initialize(nObjects);

    for (int i = 0; i < nObjects; i++) {
      cJSON* objectNode = cJSON_GetArrayItem(objectsNode, i);

      WorldMapObject* obj = malloc(sizeof(WorldMapObject));
      obj->id = cJSON_GetObjectItem(objectNode, "id")->valueint;
      obj->type = cJSON_GetObjectItem(objectNode, "type")->valueint;
      obj->x = cJSON_GetObjectItem(objectNode, "x")->valueint;
      obj->y = cJSON_GetObjectItem(objectNode, "y")->valueint;

      // todo: convert x/y to ISO

      cJSON* atlasNode = cJSON_GetObjectItem(objectNode, "atlas");

      if (atlasNode != NULL) {
        obj->atlas = atlasNode->valueint;
        obj->atlasIndex = cJSON_GetObjectItem(objectNode, "index")->valueint;
      }

      Djinni_Util_Array.insert(mapLayer->objects, obj);
    }
  }
  */
}

static void setMapDataFile(WorldMap* m, char* filename) {
  strncpy(m->mapFileName, filename, DJINNI_MAX_MAP_FILENAME);
}

static void setObjectLoader(WorldMap* w, void (*objectLoader)(World*, WorldMapObject*, DJINNI_MAP_LAYER)) {
  w->objectLoader = objectLoader;
}

static void load(WorldMap* m, Renderer* r) {
  if (m->mapFileName[0] != 0) {
    char* text = djinniReadFile(m->mapFileName);
    cJSON* root = cJSON_Parse(text);

    m->type = cJSON_GetObjectItem(root, "type")->valueint;
    m->width = cJSON_GetObjectItem(root, "width")->valueint;
    m->height = cJSON_GetObjectItem(root, "height")->valueint;

    if (m->type == ISOMETRIC_MAP_TYPE) {
      m->isometricObjects = Djinni_Util_Array.initialize(16);
    }

    cJSON* layersNode = cJSON_GetObjectItem(root, "layers");
    int nLayers = cJSON_GetArraySize(layersNode);
    for (int i = 0; i < nLayers; i++) {
      cJSON* layerNode = cJSON_GetArrayItem(layersNode, i);
      loadLayer(m, r, layerNode);
    }

    cJSON_Delete(root);
    free(text);
  }
}

static void inspect(WorldMap* m) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Map( address:(%p) type:(%d) width:(%d) height:(%d) )",
    m, m->type, m->width, m->height
  );

  for (int i = 0; i < DJINNI_MAX_MAP_LAYERS; i++) {
    WorldMapLayer* mapLayer = &(m->layers[i]);

    Djinni_Util_Logger.log_debug(
      "\tDjinni::Map::Layer( id:(%p) type:(%d) n-atlases:(%d) n-objects:(%d) )",
      mapLayer->id, mapLayer->type, (mapLayer->atlases == NULL ? 0 : mapLayer->atlases->used),
      (mapLayer->objects == NULL ? 0 : mapLayer->objects->used)
    );

    if (mapLayer->type == TILE_LAYER_TYPE) {
      Djinni_Util_Logger.log_debug(
        "\t\tDjinni::Map::Layer::Tiles( n-tiles:(%d) width:(%d) height:(%d) )",
        mapLayer->tiles.nTiles, mapLayer->tiles.tileWidth, mapLayer->tiles.tileHeight
      );

      for (int j = 0; j < mapLayer->tiles.nTiles; j++) {
        MapTile tile = mapLayer->tiles.data[j];

        if (tile.empty) { continue; }

        Djinni_Util_Logger.log_debug(
          "\t\t\tDjinni::Map::Layer::Tile( x:(%d) y:(%d) sx:(%d) sy:(%d) atlas:(%d) index:(%d) )",
          tile.x, tile.y, tile.sx, tile.sy, tile.atlasIndex, tile.tileIndex
        );
      }
    }

    if (mapLayer->objects != NULL) {
      for (int j = 0; j < mapLayer->objects->used; j++) {
        WorldMapObject* mo = mapLayer->objects->data[j];

        Djinni_Util_Logger.log_debug(
          "\t\tDjinni::Map::Layer::Object( type:(%d) x:(%d) y:(%d) )",
          mo->type, mo->x, mo->y
        );
      }
    }

  }
}

static void destroy(WorldMap* m) {
  for (int i = 0; i < DJINNI_MAX_MAP_LAYERS; i++) {
    WorldMapLayer* mapLayer = &(m->layers[i]);

    if (mapLayer->atlases != NULL) {
      for (int j = 0; j < mapLayer->atlases->used; j++) {
        Djinni_Video_Image_Atlas.destroy(mapLayer->atlases->data[j]);
      }

      Djinni_Util_Array.destroy(mapLayer->atlases, NULL);
    }

    if (mapLayer->objects != NULL) {
      Djinni_Util_Array.destroy(mapLayer->objects, free);
    }

    if (mapLayer->type == TILE_LAYER_TYPE) {
      free(mapLayer->tiles.data);
    }
  }

  if (m->isometricObjects != NULL) {
    Djinni_Util_Array.destroy(m->isometricObjects, free);
  }

  free(m);
}

struct Djinni_MapStruct Djinni_Map = {
  .create = create,
  .setMapDataFile = setMapDataFile,
  .setObjectLoader = setObjectLoader,
  .load = load,
  .inspect = inspect,
  .destroy = destroy
};