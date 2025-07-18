#ifndef DJINNI_RENDERABLE_ENTITY
#define DJINNI_RENDERABLE_ENTITY 1

#include "djinni/util/shared.h"
#include "djinni/video/shared.h"
#include "djinni/renderable/shared.h"
#include "djinni/geometry/shared.h"
#include "djinni/physics/shared.h"
#include "djinni/game/game.h"

#include "djinni/geometry/grid.h"
#include "djinni/geometry/observablePoint.h"
#include "djinni/geometry/rectangle.h"
#include "djinni/video/texture.h"
#include "djinni/physics/physics.h"
#include "djinni/renderable/shape/shape.h"
#include "djinni/util/array.h"

typedef enum {
  ENTITY_DEAD,
  ENTITY_ALIVE
} ENTITY_STATE;

typedef enum {
  ENTITY_TYPE_NONE,
  ENTITY_TYPE_SPRITE,
  ENTITY_TYPE_SHAPE,
} ENTITY_TYPE;

typedef struct EntityStruct {
  ENTITY_TYPE type;
  ENTITY_STATE status;

  int id;

  // flags (todo: convert to single int)
  int dirty;
  int keepAlive;
  int alwaysUpdate;

  // draw-size and bounds
  ObservablePoint anchorPoint;
  Rectangle bounds;

  // physic size
  PhysicsBody body;

  // grid data
  GridLocation locations[DJINNI_GRID_MAX_LEVELS];

  // Gfx
  Texture* texture;
  Shape shape;
  DjinniArray* children;

  // functions
  void (*update)(struct EntityStruct*, Game*, double dt);
  void (*onEnterViewport)(struct EntityStruct*, Game*, double dt);
  void (*onExitViewport)(struct EntityStruct*, Game*, double dt);
  void (*onCollide)(struct EntityStruct*, struct EntityStruct*, Game*, double dt);
} Entity;

struct Djinni_Renderable_EntityStruct {
  Entity (*entity)(int, int, int, int, ENTITY_TYPE);
  Entity* (*create)(int, int, int, int, ENTITY_TYPE);

  Coordinate (*getPosition)(Entity*);
  Coordinate (*getRenderPoint)(Entity*);
  void (*move)(Entity*, int, int);
  void (*setPosition)(Entity*, int, int);

  void (*setAnchor)(Entity*, float, float);

  void (*scale)(Entity*, float, float);
  int (*getRenderedWidth)(Entity*);
  int (*getRenderedHeight)(Entity*);
  int (*getBodyWidth)(Entity*);
  int (*getBodyHeight)(Entity*);

  void (*inspect)(Entity*);

  void (*arrayDestroyCallback)(void*);
  void (*destroy)(Entity*);
};

extern struct Djinni_Renderable_EntityStruct Djinni_Renderable_Entity;

#endif