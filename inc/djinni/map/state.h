#ifndef DJINNI_MAP_STATE
#define DJINNI_MAP_STATE 1

#include "djinni/map/shared.h"

void djinni_map_state_initialize();
void djinni_map_state_set_map(Djinni_Map* djinni_map);
Djinni_Map* djinni_map_state_get_map();
void djinni_map_state_destroy();

#endif