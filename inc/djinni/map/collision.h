#ifndef DJINNI_MAP_COLLISION
#define DJINNI_MAP_COLLISION 1

#include "djinni/map/shared.h"

typedef enum {
  COLLISION_DEFINITION_RECTANGLE
} COLLISION_DEFINITION_TYPE;



void djinni_map_collision_load(Djinni_Map* djinni_map, cJSON* root);

#endif