#ifndef DJINNI_MAP_COLLISION
#define DJINNI_MAP_COLLISION 1

#include "djinni/map/shared.h"
#include "djinni/util/array.h"

typedef enum {
  COLLISION_DEFINITION_RECTANGLE
} COLLISION_DEFINITION_TYPE;

typedef struct Djinni_MapCollisionShapeStruct {
  COLLISION_DEFINITION_TYPE type;

  int width;
  int height;

  int x1;
  int x2;

} Djinni_MapCollisionShape;

typedef struct Djinni_MapCollisionDefinitionStruct {
  char name[DJINNI_MAP_MAX_DEFINITION_LEN];

  DjinniArray* shapes;
} Djinni_MapCollisionDefinition;

void djinni_map_collision_definitions_load(Djinni_Map* djinni_map, cJSON* root);

#endif