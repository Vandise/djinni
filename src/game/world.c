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

  return w;
}

static void addEntity(World* w, Entity* e) {
  Djinni_Util_Logger.log_dev("Djinni::Game::World.addEntity(world:(%p), entity:(%p) entity.id:(%d) )", w, e, e->id);

  e->id = w->entities->used;

  Djinni_Util_Array.insert(w->entities, e);
}

static void removeEntity(World* w, Entity* e) {
  Djinni_Util_Logger.log_dev("Djinni::Game::World.removeEntity(world:(%p), entity:(%p) entity.id:(%d) )", w, e, e->id);

  if (e->id < 0) {
    Djinni_Util_Logger.log_warn("Djinni::Game::World.removeEntity( entity.id:(%d) ) - Entity does not exist", e->id);
    return;
  }

  Djinni_Util_Array.removeIndex(w->entities, e->id);

  Djinni_Renderable.Entity->destroy(e);
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
  .destroy = destroy
};