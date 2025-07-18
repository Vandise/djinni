#ifndef DJINNI_PHYSICS_COLLISION
#define DJINNI_PHYSICS_COLLISION 1

#include "djinni/game/shared.h"
#include "djinni/physics/shared.h"
#include "djinni/geometry/shared.h"

struct Djinni_Physics_CollisionStruct {
  void (*tick)(Game*, Entity*, GridCell*, int, double);
};

extern struct Djinni_Physics_CollisionStruct Djinni_Physics_Collision;

#endif