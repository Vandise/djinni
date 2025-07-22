#include "djinni/util/util.h"
#include "djinni/game/game.h"

static World* create(WorldSettings settings) {
  World* w = malloc(sizeof(World));

  w->settings = settings;
  w->entities = Djinni_Util_Array.initialize(10);
  w->grid = Djinni_Geometry_Grid.create(
    settings.gridCellCapacity,
    settings.width,
    settings.height,
    settings.finestGridSize,
    settings.mediumGridSize,
    settings.coarseGridSize
  );
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

static void removeEntity(World* w, Entity* e) {
  Djinni_Util_Logger.log_dev("Djinni::Game::World.removeEntity(world:(%p), entity:(%p) entity.id:(%d) )", w, e, e->id);

  int id = e->id;

  if (id < 0) {
    Djinni_Util_Logger.log_warn("Djinni::Game::World.removeEntity( entity.id:(%d) ) - Entity does not exist", id);
    return;
  }

  Djinni_Geometry_Grid.removeEntity(w->grid, e);
  Djinni_Util_Array.removeIndex(w->entities, e->id);
  Djinni_Renderable.Entity->destroy(e);

  for (int i = id; i < w->entities->used; i++) {
    Entity* entity = w->entities->data[i];
    entity->id = i;
  }
}

static void update(Game* game, ViewportBounds viewport, DJINNI_RING ring, double dt) {
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
    removeEntity(game->world, e);
  }

  Djinni_Util_Array.destroy(entitiesToDelete, NULL);
}

static void destroy(World* w) {
  Djinni_Util_Logger.log_dev("Djinni::Game::World.destroy( address:(%p) )", w);

  Djinni_Geometry_Grid.destroy(w->grid);

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
  .update = update,
  .destroy = destroy
};