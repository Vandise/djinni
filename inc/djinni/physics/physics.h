#ifndef DJINNI_PHYSICS
#define DJINNI_PHYSICS 1

#include "djinni/physics/shared.h"
#include "djinni/game/shared.h"
#include "djinni/geometry/grid.h"
#include "djinni/physics/body.h"
#include "djinni/physics/collision.h"

struct Djinni_PhysicsStruct {
  struct Djinni_Physics_BodyStruct* Body;
  struct Djinni_Physics_CollisionStruct* Collision;

  void (*initialize)();
  void (*tick)(Game*, double, DJINNI_RING);
};

extern struct Djinni_PhysicsStruct Djinni_Physics;

#endif