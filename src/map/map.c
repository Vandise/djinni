#include "djinni/util/util.h"
#include "djinni/map/map.h"
#include "djinni/game/camera.h"
#include "djinni/util/json.h"

static int getIndex(int x, int y, int maxX) {
  return y * maxX + x;
}

static WorldMap* create() {
  WorldMap* m = malloc(sizeof(WorldMap));

  m->mapFileName[0] = 0;

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

      mt->layer = layerId;
      mt->tileIndex = cJSON_GetObjectItem(tileNode, "index")->valueint;
      mt->atlasIndex = cJSON_GetObjectItem(tileNode, "atlas")->valueint;
      mt->width = mapLayer->tiles.tileWidth;
      mt->height = mapLayer->tiles.tileHeight;

      mt->x = i % mapLayer->tiles.nxTiles;
      mt->y = i / mapLayer->tiles.nxTiles;

      if (m->type == ISOMETRIC_MAP_TYPE) {
        // object->sx = ((x * m->tileWidth / 2) + (y * m->tileWidth / 2));
        mt->sx = ((mt->x * mapLayer->tiles.tileWidth / 2) + (mt->y * mapLayer->tiles.tileWidth / 2));
        // object->sy = (m->height / 2) - ((y * (m->tileHeight / 2) / 2) - (x * (m->tileHeight / 2) / 2));
        mt->sy = (m->height / 2) - ((mt->y * (mapLayer->tiles.tileHeight / 2) / 2) - (mt->x * (mapLayer->tiles.tileHeight / 2) / 2));
        mt->x = mt->sx;
        mt->y = mt->sy;
      } else {
        mt->sx = mt->x;
        mt->sy = mt->y;
      }
    }
  }

  //
  // Objects
  //
  cJSON* objectsNode = cJSON_GetObjectItem(layerNode, "objects");
  if (objectsNode != NULL) {
    int nObjects = cJSON_GetArraySize(objectsNode);
    mapLayer->objects = Djinni_Util_Array.initialize(nObjects);

    for (int i = 0; i < nObjects; i++) {
      cJSON* objectNode = cJSON_GetArrayItem(objectsNode, i);
    }
  }
}

static void setMapDataFile(WorldMap* m, char* filename) {
  strncpy(m->mapFileName, filename, DJINNI_MAX_MAP_FILENAME);
}

static void load(WorldMap* m, Renderer* r) {
  if (m->mapFileName[0] != 0) {
    char* text = djinniReadFile(m->mapFileName);
    cJSON* root = cJSON_Parse(text);

    m->type = cJSON_GetObjectItem(root, "type")->valueint;
    m->width = cJSON_GetObjectItem(root, "width")->valueint;
    m->height = cJSON_GetObjectItem(root, "height")->valueint;

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
      "\tDjinni::Map::Layer( id:(%p) type:(%d) n-atlases:(%d) )",
      mapLayer->id, mapLayer->type, (mapLayer->atlases == NULL ? 0 : mapLayer->atlases->used)
    );

    if (mapLayer->type == TILE_LAYER_TYPE) {
      Djinni_Util_Logger.log_debug(
        "\t\tDjinni::Map::Layer::Tiles( n-tiles:(%d) width:(%d) height:(%d) )",
        mapLayer->tiles.nTiles, mapLayer->tiles.tileWidth, mapLayer->tiles.tileHeight
      );

      for (int j = 0; j < mapLayer->tiles.nTiles; j++) {
        MapTile tile = mapLayer->tiles.data[j];
          Djinni_Util_Logger.log_debug(
            "\t\t\tDjinni::Map::Layer::Tile( x:(%d) y:(%d) sx:(%d) sy:(%d) atlas:(%d) index:(%d) )",
            tile.x, tile.y, tile.sx, tile.sy, tile.atlasIndex, tile.tileIndex
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

  free(m);
}

struct Djinni_MapStruct Djinni_Map = {
  .create = create,
  .setMapDataFile = setMapDataFile,
  .load = load,
  .inspect = inspect,
  .destroy = destroy
};