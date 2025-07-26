#include "djinni/util/util.h"
#include "djinni/game/game.h"
#include "djinni/geometry/isometric.h"

static World* create(WorldSettings settings) {
  World* w = malloc(sizeof(World));

  w->settings = settings;
  w->entities = Djinni_Util_Array.initialize(10);
  w->grid = NULL;
  w->worldMap = NULL;

/*
    TODO:

    frame #5: 0x00000001000071f0 example`initialize(nElements=10) at array.c:12:17
    frame #6: 0x000000010000d70a example`initializeLevel(level=0x0000000102027610, cellSize=128, cellCapacity=10, worldWidth=1966079, worldHeight=6293388) at grid.c:58:32
    frame #7: 0x000000010000cd9b example`create(cellCapacity=10, worldWidth=1966079, worldHeight=6293388, finestSize=32, midSize=64, coarseSize=128) at grid.c:35:5
    frame #8: 0x00000001000096e4 example`setWorldMap(w=0x0000000102035bb0, wm=0x0000000102028b40) at world.c:114:15
    frame #9: 0x00000001000017e6 example`onStageCreate(self=0x00000001020270d0, game=0x0000000102819a00, previous=0x0000000000000000) at stage.c:23:3
    frame #10: 0x000000010000887e example`changeStage(game=0x0000000102819a00, id=0) at game.c:70:5
    frame #11: 0x000000010000845c example`start(game=0x0000000102819a00, level=0) at djinni.c:61:3
    frame #12: 0x0000000100001caa example`main at main.c:51:3
*/

  if (settings.type != MAP_WORLD_TYPE) {
    w->grid = Djinni_Geometry_Grid.create(
      settings.gridCellCapacity,
      settings.width,
      settings.height,
      settings.finestGridSize,
      settings.mediumGridSize,
      settings.coarseGridSize
    );
  }

  w->camera = NULL;

  return w;
}

static void addEntity(World* w, Entity* e) {
  Djinni_Util_Logger.log_dev("Djinni::Game::World.addEntity(world:(%p), entity:(%p) entity.id:(%d) )", w, e, e->id);

  e->id = w->entities->used;

  Djinni_Util_Array.insert(w->entities, e);

  //
  // if there is a map and it is isometric, it's coordinates need to be translated
  // relative to the isometric grid size and added to the isometric objects draw list
  //
  WorldMap* m = w->worldMap;

  if (m != NULL && m->type == ISOMETRIC_MAP_TYPE) {  
    Coordinate cartesianPos = Djinni_Renderable.Entity->getRenderPoint(e);    
    Coordinate isoxy = Djinni_Geometry_Isometric.cartesianToISO(
      m->height, m->width, m->baseTileWidth, m->baseTileHeight, cartesianPos.x, cartesianPos.y
    );

    printf(
      "Adding entity to world map: %p  rx:%d ry:%d x:%d y:%d map-h:%d tw:%d th:%d \n",
      e, cartesianPos.x, cartesianPos.y, isoxy.x, isoxy.y, m->height, m->baseTileWidth, m->baseTileHeight
    );

    Djinni_Renderable.Entity->setPosition(e, isoxy.x, isoxy.y);

    IsometricObject* obj = malloc(sizeof(IsometricObject));
      obj->type = ISOMETRIC_ENTITY_TYPE;
      obj->layer = &(e->worldMapData->layer);
      obj->x = &(e->body.bounds.instance.x);
      obj->y = &(e->body.bounds.instance.y);
      obj->width = &(e->body.bounds.instance.w);
      obj->height = &(e->body.bounds.instance.h);
      obj->entity = e;
    e->isoRef = obj;

    Djinni_Util_Array.insert(m->isometricObjects, obj);
  }

  DJINNI_RING ring = Djinni_Geometry_Grid.computeRingLevel(
    Djinni_Game.Camera->getViewportBounds(w->camera),
    e
  );

  Djinni_Geometry_Grid.insert(w->grid, e, ring);
}

static void removeEntity(Game* game, Entity* e, double dt) {
  World* w = game->world;

  Djinni_Util_Logger.log_dev("Djinni::Game::World.removeEntity(world:(%p), entity:(%p) entity.id:(%d) )", w, e, e->id);

  int id = e->id;

  if (id < 0) {
    Djinni_Util_Logger.log_warn("Djinni::Game::World.removeEntity( entity.id:(%d) ) - Entity does not exist", id);
    return;
  }

  Djinni_Geometry_Grid.removeEntity(w->grid, e);
  Djinni_Util_Array.removeIndex(w->entities, e->id);

  if (e->onDestroy != NULL) {
    e->onDestroy(e, game, dt);
  }

  Djinni_Renderable.Entity->destroy(e);

  for (int i = id; i < w->entities->used; i++) {
    Entity* entity = w->entities->data[i];
    entity->id = i;
  }
}

static void setWorldMap(World* w, WorldMap* wm) {
  if (w->settings.type == MAP_WORLD_TYPE) {
    w->worldMap = wm;

    w->settings.width = wm->width;
    w->settings.height = wm->height;

    w->grid = Djinni_Geometry_Grid.create(
      w->settings.gridCellCapacity,
      w->settings.width,
      w->settings.height,
      w->settings.finestGridSize,
      w->settings.mediumGridSize,
      w->settings.coarseGridSize
    );
  }
}

static int drawComparator(const void *a, const void *b) {
  int result;
  IsometricObject *t1, *t2;

  t1 = *(IsometricObject**)a;
  t2 = *(IsometricObject**)b;

  result = (*(t1->layer)) - (*(t2->layer));

  if (result == 0) {
    result = (*(t1->y)) - (*(t2->y));

    if (result == 0) {
      result = (*(t1->x) + (*(t1->width))) - (*(t2->x) + (*(t2->width)));
    }
  }

  return result;
}

static void draw(Renderer* r, Game* game, double dt) {
  WorldMap* m = game->world->worldMap;
  Camera* c = game->camera;
  ViewportBounds viewport = Djinni_Camera.getViewportBounds(c);

  if (m->type == ISOMETRIC_MAP_TYPE) {
    qsort(
      m->isometricObjects->data,
      m->isometricObjects->used,
      sizeof(void*),
      drawComparator
    );

    for (int i = 0; i < m->isometricObjects->used; i++) {
      IsometricObject* object = m->isometricObjects->data[i];

      int x = *(object->x);
      int y = *(object->y);
      int w = *(object->width);
      int h = *(object->height);
      int layer = *(object->layer);

      if (
        x > viewport.x1 - (w * 2) && x < viewport.x2 + (w * 2) &&
        y > viewport.y1 - (h * 2) && y < viewport.y2 + (h * 2)
      ) {

        //
        // Tiles
        //  todo: move this to renderable
        //
        if (object->type == ISOMETRIC_TILE_TYPE) {
          WorldMapLayer* mapLayer = &(m->layers[layer]);

          AtlasImage* img = Djinni_Video_Image_Atlas.getIndex(
            mapLayer->atlases->data[object->tile->atlasIndex],
            object->tile->tileIndex
          );

          Djinni_Video_Image_Atlas.blit(
            r,
            img,
            x - c->point.x,
            y - c->point.y,
            w,
            h
          );

          continue;
        }

        if (object->type == ISOMETRIC_ENTITY_TYPE && object->entity != NULL) {
          Djinni_Renderable_Paint.entity(r, object->entity, c);
          continue;
        }
      }
    }
  }

  /*

  for (int i = 0; i < DJINNI_MAX_MAP_LAYERS; i++) {
    WorldMapLayer* mapLayer = &(m->layers[i]);

    //
    // Draw Entities only in the fine ring
    //
    if (mapLayer->id == ENTITY_LAYER) {
      Djinni_Renderable.draw(r, game->world->grid, game->camera, DJINNI_RING_FINE);
      continue;
    }

    //
    // Tile Layers
    //
    if (mapLayer->type == TILE_LAYER_TYPE) {
      qsort(mapLayer->tiles.data, mapLayer->tiles.nTiles, sizeof(MapTile), drawComparator);

      for (int i = 0; i < mapLayer->tiles.nTiles; i++) {
        MapTile tile = mapLayer->tiles.data[i];

        if (tile.empty) { continue; }

        //
        // draw tiles only in the viewport with some padding
        //
        if (
          tile.sx > viewport.x1 - (tile.width * 2) && tile.sx < viewport.x2 + (tile.width * 2) &&
          tile.sy > viewport.y1 - (tile.height * 2) && tile.sy < viewport.y2 + (tile.height * 2)
        ) {
          AtlasImage* img = Djinni_Video_Image_Atlas.getIndex(
            mapLayer->atlases->data[tile.atlasIndex],
            tile.tileIndex
          );

          Djinni_Video_Image_Atlas.blit(
            r,
            img,
            tile.sx - c->point.x,
            tile.sy - c->point.y,
            tile.width,
            tile.height
          );
        }
      }
      continue;
    }
  }
  */
}

static void update(Game* game, ViewportBounds viewport, DJINNI_RING ring, double dt) {
  if (game->world->grid == NULL) { return; }

  GridLevel* level = &game->world->grid->levels[ring];
  DjinniArray* entitiesToDelete = Djinni_Util_Array.initialize(16);

  for (int y = 0; y < level->height; y++) {
    for (int x = 0; x < level->width; x++) {
      GridCell* cell = &level->cells[y * level->width + x];

      if (cell->entities->used > 0) {
        for (int i = 0; i < cell->entities->used; i++) {
          Entity* subject = cell->entities->data[i];
          if (subject->deleteFlag) { continue; }

          //
          // if the entity is destoryed, remove from the entities list and grid
          // decrement i so the shift is accounted for
          //
          if (subject->status == ENTITY_DESTORYED && !subject->deleteFlag) {
            subject->deleteFlag = 1;
            Djinni_Util_Array.insert(entitiesToDelete, subject);
            continue;
          }

          if (subject->update != NULL) {
            subject->update(subject, game, dt);
          }

          //
          // Ring updates due to camera scrolls or movement
          //
          DJINNI_RING expectedRing = Djinni_Geometry_Grid.computeRingLevel(viewport, cell->entities->data[i]);
          DJINNI_RING currentRing = Djinni_Geometry_Grid.getCurrentEntityRing(cell->entities->data[i]);

          if (expectedRing != currentRing) {
            Djinni_Util_Logger.log_dev(
              "Djinni::Geometry::Grid.update( grid:(%p), entity:(%p) ring:(%d) nextRing:(%d) )",
              game->world->grid, cell->entities->data[i], currentRing, expectedRing
            );

            Djinni_Geometry_Grid.removeEntity(game->world->grid, subject);
            Djinni_Geometry_Grid.insert(game->world->grid, subject, expectedRing);

            if (expectedRing == DJINNI_RING_FINE || currentRing == DJINNI_RING_FINE) {
              if(expectedRing == DJINNI_RING_FINE && subject->onEnterViewport != NULL) {
                subject->onEnterViewport(subject, game, dt);
                continue;
              }

              if (currentRing == DJINNI_RING_FINE && expectedRing != DJINNI_RING_FINE && subject->onExitViewport != NULL) {
                subject->onExitViewport(subject, game, dt);
              }
            }
          } else {
            if (subject->dirty && Djinni_Geometry_Grid.entityNeedsCellUpdate(game->world->grid, subject)) {
              Djinni_Geometry_Grid.removeEntity(game->world->grid, subject);
              Djinni_Geometry_Grid.insert(game->world->grid, subject, expectedRing);
            }
          }

          //
          // movement cleanup
          //
          subject->dirty = 0;
        }
      }

    }
  }

  for (int i = 0; i < entitiesToDelete->used; i++) {
    Entity* e = (Entity*)entitiesToDelete->data[i];
    removeEntity(game, e, dt);
  }

  Djinni_Util_Array.destroy(entitiesToDelete, NULL);
}

static void destroy(World* w) {
  Djinni_Util_Logger.log_dev("Djinni::Game::World.destroy( address:(%p) )", w);

  if (w->worldMap != NULL) {
    Djinni_Map.destroy(w->worldMap);
  }

  if (w->grid != NULL) {
    Djinni_Geometry_Grid.destroy(w->grid);
  }

  Djinni_Util_Array.destroy(
    w->entities,
    Djinni_Renderable.Entity->arrayDestroyCallback
  );

  free(w);
}

struct Djinni_Game_GameWorldStruct Djinni_World = {
  .create = create,
  .addEntity = addEntity,
  .removeEntity = removeEntity,
  .setWorldMap = setWorldMap,
  .update = update,
  .draw = draw,
  .destroy = destroy
};