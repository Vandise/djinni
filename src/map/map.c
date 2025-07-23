#include "djinni/util/util.h"
#include "djinni/map/map.h"

static int getIndex(int x, int y, int maxX) {
  return y * maxX + x;
}

static void worldMapToISO(WorldMap* m) {
	for (int x = 0; x < m->nXTiles; x++) {
		for (int y = 0; y < m->nYTiles; y++) {

      int index = getIndex(x, y, m->nXTiles);

      MapObject* object = &(m->objects[index]);

      object->sx = object->x = ((x * m->tileWidth / 2) + (y * m->tileWidth / 2));
      object->sy = object->y = ((y * (m->tileHeight / 2) / 2) - (x * (m->tileHeight / 2) / 2));
      object->texture = m->tiles->data[ m->data[index].tileId ];
		}
	}
}

/*
  this will eventually read from a file and have multiple rendering levels
*/
static WorldMap* create(int width, int height, int tileWidth, int tileHeight, DJINNI_MAP_TYPE type) {
  WorldMap* m = malloc(sizeof(WorldMap));
  m->type = type;
  m->width = width;
  m->height = height;
  m->nObjects = 0;

  m->tileWidth = tileWidth;
  m->tileHeight = tileHeight;

  m->nXTiles = (width / tileWidth);
  m->nYTiles = (height / tileHeight);

  m->tiles = Djinni_Util_Array.initialize(16);
  m->data = malloc(sizeof(MapTile) * m->nXTiles * m->nYTiles);
  m->objects = malloc(sizeof(MapObject) * m->nXTiles * m->nYTiles);

  return m;
}

static void load(WorldMap* m, Renderer* r) {
	for (int x = 0; x < m->nXTiles; x++) {
		for (int y = 0; y < m->nYTiles; y++) {
      //
      // testing a tile/texture id
      //
      int id = 0;
		  int index = getIndex(x, y, m->nXTiles);
      m->data[index].tileId = id;

      if (m->tiles->used == 0) {
        Djinni_Util_Array.insert(m->tiles, Djinni_Video_Texture.load(r, "gfx/greycube.png"));
      }

      m->nObjects++;
		}
	}

  if (m->type == ISOMETRIC_MAP_TYPE) {
    worldMapToISO(m);
  }
}

static int drawComparator(const void *a, const void *b) {
  MapObject *o1, *o2;
  
  o1 = (MapObject*) a;
  o2 = (MapObject*) b;
  
  return o1->y - o2->y;
}

static void draw(WorldMap* m, Renderer* r, double dt) {
  MapObject *o;

  qsort(m->objects, m->nObjects, sizeof(MapObject), drawComparator);

  for (int i = 0; i < m->nObjects; i++) {
    o = &(m->objects[i]);
    Djinni_Video_Texture.blit(r, o->texture, o->sx, o->sy, m->tileWidth, m->tileHeight);
  }
}

static void inspect(WorldMap* m) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Map( address:(%p) type:(%d) width:(%d) height:(%d) ))",
    m, m->type, m->width, m->height
  );
}

static void destroy(WorldMap* m) {
  Djinni_Util_Array.destroy(m->tiles, Djinni_Video_Texture.arrayDestroyCallback);
  free(m->data);
  free(m->objects);
  free(m);
}

struct Djinni_MapStruct Djinni_Map = {
  .create = create,
  .load = load,
  .draw = draw,
  .inspect = inspect,
  .destroy = destroy
};