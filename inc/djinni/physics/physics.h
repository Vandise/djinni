#ifndef DJINNI_PHYSICS
#define DJINNI_PHYSICS 1

#include "djinni/physics/shared.h"
#include "djinni/physics/body.h"

struct Djinni_PhysicsStruct {
  struct Djinni_Physics_BodyStruct* Body;

  void (*initialize)();
};

extern struct Djinni_PhysicsStruct Djinni_Physics;

#endif