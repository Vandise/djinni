#include "djinni/util/util.h"
#include "djinni/physics/collision.h"
#include "djinni/renderable/renderable.h"
#include "djinni/game/game.h"

static int rectangleOverlaps(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) {
  return ax < bx + bw && ax + aw > bx &&
         ay < by + bh && ay + ah > by;
}

static void tick(Game* game, Entity* entity, GridCell* cell, int index, double dt) {
  if (cell->entities->used > 0) {
    for (int i = 0; i < cell->entities->used; i++) {
      Entity* other = cell->entities->data[i];

      if (entity == other || i == index) {
        continue;
      }

      Rectangle o = other->body.bounds;
      Rectangle e = entity->body.bounds;

      if (
        rectangleOverlaps(e.instance.x, e.instance.y, e.instance.w, e.instance.h,
                     o.instance.x, o.instance.y, o.instance.w, o.instance.h)
      ) {
        Djinni_Util_Logger.log_dev("Djinni::Physics::Collision.tick( entity:(%p) collides with entity:(%p) )", entity, other);

        //
        // todo: core physics
        //

        if (entity->onCollide != NULL) {
          entity->onCollide(entity, other, game, dt);
        }
      }
    }
  }
}

struct Djinni_Physics_CollisionStruct Djinni_Physics_Collision = {
  .tick = tick
};