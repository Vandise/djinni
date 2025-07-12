#include "djinni/util/util.h"
#include "djinni/physics/physics.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Physics.initialize");

  Djinni_Physics.Body = &Djinni_Physics_Body;
}

struct Djinni_PhysicsStruct Djinni_Physics = {
  .initialize = initialize
};