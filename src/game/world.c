#include "djinni/util/util.h"
#include "djinni/game/game.h"

static World* create(WorldSettings settings) {
  World* w = malloc(sizeof(World));

  w->settings = settings;
  w->entities = Djinni_Util_Array.initialize(10);
  w->grid = NULL;
  w->worldMap = NULL;

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

    //
    // todo: add entities to the grid from the entities layer
    //
  }
}

static int drawComparator(const void *a, const void *b) {
  int result;
  MapTile *t1, *t2;

  t1 = (MapTile*) a;
  t2 = (MapTile*) b;

  result = t1->layer - t2->layer;

  if (result == 0) {
    result = t1->y - t2->y;

    if (result == 0) {
      result = (t1->sx + t1->width) - (t2->sx + t2->width);
    }
  }

  return result;
}

static void draw(Renderer* r, Game* game, double dt) {
  WorldMap* m = game->world->worldMap;
  Camera* c = game->camera;
  ViewportBounds viewport = Djinni_Camera.getViewportBounds(c);

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