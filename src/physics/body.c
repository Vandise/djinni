#include "djinni/util/util.h"
#include "djinni/physics/body.h"

static PhysicsBody create(int x, int y, int w, int h) {
  PhysicsBody body;

  body.bounds = Djinni_Geometry_Rectangle.create(
    x,y,w,h
  );

  body.velocity.dx = body.velocity.dy = 0;

  return body;
}

static void inspect(PhysicsBody* b) {
  Djinni_Util_Logger.log_debug(
    "Djinni::Physics::PhysicsBody( address:(%p))", b
  );
  Djinni_Geometry_Rectangle.inspect(&(b->bounds));
}

struct Djinni_Physics_BodyStruct Djinni_Physics_Body = {
  .create = create
};