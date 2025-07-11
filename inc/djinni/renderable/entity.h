#ifndef DJINNI_RENDERABLE_ENTITY
#define DJINNI_RENDERABLE_ENTITY 1

#include "djinni/video/shared.h"
#include "djinni/renderable/shared.h"
#include "djinni/geometry/shared.h"
#include "djinni/physics/shared.h"

#include "djinni/geometry/rectangle.h"
#include "djinni/video/texture.h"
#include "djinni/physics/physics.h"

typedef enum {
  ENTITY_DEAD,
  ENTITY_ALIVE
} ENTITY_STATE;

typedef enum {
  ENTITY_TYPE_NONE,
  ENTITY_TYPE_SPRITE
} ENTITY_TYPE;

typedef struct EntityStruct {
  ENTITY_TYPE type;
  ENTITY_STATE status;
  int keepAlive;
  int alwaysUpdate;

  // draw-size bounds
  Rectangle bounds;

  // physic size
  PhysicsBody body;

  Texture* texture;
} Entity;

struct Djinni_Renderable_EntityStruct {
  Entity* (*create)(int, int, int, int, ENTITY_TYPE);
  Coordinate (*getPosition)(Entity*);
  void (*move)(Entity*, int, int);
  void (*setPosition)(Entity*, int, int);
  void (*inspect)(Entity*);
  void (*destroy)(Entity*);
};

extern struct Djinni_Renderable_EntityStruct Djinni_Renderable_Entity;

#endif