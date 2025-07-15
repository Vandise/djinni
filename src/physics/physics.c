#include "djinni/util/util.h"
#include "djinni/physics/physics.h"
#include "djinni/renderable/renderable.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Physics.initialize");

  Djinni_Physics.Body = &Djinni_Physics_Body;
}

static void tick(Entity* entity, double dt) {
  Djinni_Physics_Body.tickVelocity(entity, dt);
}

struct Djinni_PhysicsStruct Djinni_Physics = {
  .initialize = initialize,
  .tick = tick
};