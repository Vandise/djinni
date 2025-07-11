#ifndef DJINNI_PHYSICS_BODY
#define DJINNI_PHYSICS_BODY 1

#include "djinni/physics/shared.h"
#include "djinni/geometry/rectangle.h"

typedef struct PhysicsVelocityStruct {
  int dx;
  int dy;
} PhysicsVelocity;

typedef struct PhysicsBodyStruct {
  Rectangle bounds;
  PhysicsVelocity velocity;
} PhysicsBody;

struct Djinni_Physics_BodyStruct {
  PhysicsBody (*create)(int, int, int, int);
  void (*inspect)(PhysicsBody*);
};

extern struct Djinni_Physics_BodyStruct Djinni_Physics_Body;

#endif