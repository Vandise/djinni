#include "djinni/util/util.h"
#include "djinni/physics/body.h"
#include "djinni/renderable/renderable.h"

static PhysicsBody create(int x, int y, int w, int h) {
  PhysicsBody body;

  body.bounds = Djinni_Geometry_Rectangle.create(
    x,y,w,h
  );

  body.velocity.dt = body.velocity.dx = body.velocity.dy = 0;

  return body;
}


static void tickVelocity(Entity* entity, double dt) {
  if (entity->body.velocity.dx != 0 || entity->body.velocity.dy != 0) {
    if (dt - entity->body.velocity.dt >= 1) {
      Djinni_Renderable.Entity->move(
        entity, entity->body.velocity.dx, entity->body.velocity.dy
      );
      entity->body.velocity.dt = dt;
    }
  }
}

static void inspect(PhysicsBody* b) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Physics::PhysicsBody( address:(%p))", b
  );
  Djinni_Geometry_Rectangle.inspect(&(b->bounds));
}

struct Djinni_Physics_BodyStruct Djinni_Physics_Body = {
  .create = create,
  .tickVelocity = tickVelocity,
  .inspect = inspect
};